const http = require('http')
const { WebSocketServer } = require('ws')
const Y = require('yjs')
const syncProtocol = require('y-protocols/sync')
const encoding = require('lib0/encoding')
const decoding = require('lib0/decoding')

const PORT = process.env.PORT || 4444

// In-memory Yjs documents keyed by room name
const docs = new Map()

function getDoc(name) {
  if (!docs.has(name)) {
    const doc = new Y.Doc()
    docs.set(name, { doc, conns: new Set() })
  }
  return docs.get(name)
}

const MSG_SYNC = 0
const MSG_AWARENESS = 1

function send(conn, msg) {
  if (conn.readyState === 1) conn.send(msg)
}

const server = http.createServer((req, res) => {
  if (req.url === '/health') {
    res.writeHead(200)
    res.end(JSON.stringify({ status: 'ok', rooms: docs.size }))
    return
  }
  res.writeHead(200)
  res.end('Keep Sync Server')
})

const wss = new WebSocketServer({ server })

wss.on('connection', (conn, req) => {
  // Room name from URL path: /room-name
  const roomName = req.url?.slice(1) || 'default'
  const room = getDoc(roomName)
  room.conns.add(conn)

  console.log(`[${roomName}] client connected (${room.conns.size} total)`)

  // Send initial sync step 1
  const encoder = encoding.createEncoder()
  encoding.writeVarUint(encoder, MSG_SYNC)
  syncProtocol.writeSyncStep1(encoder, room.doc)
  send(conn, encoding.toUint8Array(encoder))

  conn.on('message', (data) => {
    const msg = new Uint8Array(data)
    const decoder = decoding.createDecoder(msg)
    const msgType = decoding.readVarUint(decoder)

    if (msgType === MSG_SYNC) {
      const encoder = encoding.createEncoder()
      encoding.writeVarUint(encoder, MSG_SYNC)
      syncProtocol.readSyncMessage(decoder, encoder, room.doc, conn)
      if (encoding.length(encoder) > 1) {
        send(conn, encoding.toUint8Array(encoder))
      }

      // Broadcast to other clients in the same room
      if (msg.length > 2) {
        for (const c of room.conns) {
          if (c !== conn) send(c, msg)
        }
      }
    }
  })

  conn.on('close', () => {
    room.conns.delete(conn)
    console.log(`[${roomName}] client disconnected (${room.conns.size} total)`)
    if (room.conns.size === 0) {
      // Keep doc in memory for a while for reconnections
      setTimeout(() => {
        const r = docs.get(roomName)
        if (r && r.conns.size === 0) {
          docs.delete(roomName)
          console.log(`[${roomName}] room cleaned up`)
        }
      }, 60000)
    }
  })
})

server.listen(PORT, '0.0.0.0', () => {
  console.log(`Keep Sync Server running on port ${PORT}`)
})
