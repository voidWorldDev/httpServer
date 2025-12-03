# HTTP SERVER 

Developing http server from scratch form the taking the socket programing to full http server


Phase 1 — Foundations: Raw TCP Sockets
Goal: Understand how to set up basic communication.

Tasks
- Create a server that:
  - Creates a socket (socket())
  - Binds to an address and port (bind())
  - Listens for connections (listen())
  - Accepts a client (accept())
  - Receives bytes (recv())
  - Sends bytes (send())

- Create a simple client that:
  - Connects to the server (connect())
  - Sends a message
  - Receives a response

Milestones
- Echo server (server sends back whatever client sends).
- Graceful shutdown and error handling.


Phase 2 — Concurrent Handling of Clients
Goal: Handle multiple clients efficiently.

Possible approaches
- thread-per-connection model  
  Easy to implement; not scalable for large traffic.

- thread pool model  
  Better performance; avoid creating too many threads.

- event-driven model (epoll/kqueue/select)  
  Ideal for high-performance servers.

Milestones
- You choose one concurrency approach and build a server that handles many simultaneous echo clients.


Phase 3 — Parsing HTTP
Goal: Understand and implement the HTTP 1.0/1.1 protocol.

Tasks
- Read from the socket until end of HTTP request
- Detect HTTP header termination (\r\n\r\n)
- Parse:
  - Request line (GET /path HTTP/1.1)
  - Headers (Host, Connection, Content-Type, etc.)
  - Optional body (for POST)

- Create internal structures:
  - HttpRequest object (method, path, headers, body)
  - HttpResponse object (status, headers, body)

Milestones
- Return a fixed HTTP response (200 OK, text/html).
- Support basic GET requests.


Phase 4 — Routing & Content Serving
Goal: Convert your server into a usable HTTP application platform.

Tasks
- Create a router
- Map paths to handler functions:
  server.route("/hello", GET, helloHandler);

- Serve static files
  - Open file
  - Read and send content
  - Set proper MIME types (Content-Type)

- Support POST requests
  - Parse request body
  - Pass it to handler

Milestones
- / → index.html
- /api/hello → returns JSON
- /static/* → serve files


Phase 5 — HTTP Features (1.0 / 1.1)
Goal: Support common HTTP behavior.

Key features
- Connection keep-alive  
  Reuse same socket for multiple requests
- Chunked transfer encoding (optional, advanced)
- Content-Length handling
- Query parameter parsing

Milestones
- Browser can load your server without timing out.
- Multiple files load without closing connection every time.


Phase 6 — Improved Performance
Goal: Make server stable and fast.

Upgrades
- Thread pool / async event loop  
  Use epoll, poll, or select

- Zero-copy file serving  
  Use sendfile() on Linux

- Request buffering  
  Prevent partial packet issues

- Stress testing  
  Tools: wrk, ab, siege

Milestones
- Server handles 1k–10k concurrent connections reliably.


Phase 7 — Security & Hardening
Goal: Make server production-ready.

Features
- Input sanitization
- Prevent directory traversal  
  Disallow /../../etc/passwd

- Rate limiting / IP restrictions (optional)

- TLS/HTTPS  
  Integrate with OpenSSL  
  Wrap your sockets in SSL context

Milestones
- Your server passes basic security audits.
- Serves secure HTTPS.
