FROM node:18-alpine
WORKDIR /app
COPY package.json package-lock.json ./
RUN npm ci --production
COPY index.js .
EXPOSE 4444
CMD ["node", "index.js"]
