#ifndef CONNECTION_HANDLER_HPP
#define CONNECTION_HANDLER_HPP

class ConnectionHandler {
public:
  static void startListener();

  template <typename T1, typename T2>
  static void handleRequest(const T1 &request, T2 &socket);

  template <typename T1>
  static void handleResponse(T1 &socket, unsigned int requestVersion);
};
#endif // CONNECTION_HANDLER_HPP
