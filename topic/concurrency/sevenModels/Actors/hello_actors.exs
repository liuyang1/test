defmodule Talker do
  def loop do
    receive do
      {:greet, name} -> IO.puts("Hello #{name}")
      {:praise, name} -> IO.puts("#{name}, you're amazing")
      {:celebrate, name, age} -> IO.puts("Here's to another #{age} years, #{name}")
# how to exit thread, process or actor
      {:shutdown} -> exit(:normal)
    end
    loop
  end
end

Process.flag(:trap_exit, true)
# pid = spawn(&Talker.loop/0)
pid = spawn_link(&Talker.loop/0)
send(pid, {:greet, "Yang"})
send(pid, {:praise, "Yang"})
send(pid, {:celebrate, "Yang", 29})
send(pid, {:shutdown})
# sleep(100)

receive do
  # ^ => means this value should be equal to `pid`
  {:EXIT, ^pid, reason} -> IO.puts(">>> Talker has exited (#{reason})")
end
