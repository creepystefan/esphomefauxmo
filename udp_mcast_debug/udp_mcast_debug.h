#pragma once

#include "esphome/core/defines.h"
#ifdef USE_NETWORK
#include "esphome/core/helpers.h"
#include "esphome/components/network/ip_address.h"

#if defined(USE_SOCKET_IMPL_BSD_SOCKETS) || defined(USE_SOCKET_IMPL_LWIP_SOCKETS)
#include "esphome/components/socket/socket.h"
#endif
#ifdef USE_SOCKET_IMPL_LWIP_TCP
#include <WiFiUdp.h>
#endif
#include <array>
#include <initializer_list>
#include <span>
#include <vector>

#include <string>

#define FAUXMO_DEVICE_UNIQUE_ID_LENGTH  27
#define FAUXMO_TCP_MAX_CLIENTS      10
#define FAUXMO_UDP_MULTICAST_IP     IPAddress(239,255,255,250)

namespace esphome::udp_mcast_debug {

typedef std::function<void(unsigned char, const char *, bool, unsigned char)> TSetStateCallback;
typedef std::function<void(unsigned char, const char *, bool, unsigned char, std::byte *)> TSetStateWithColorCallback;

typedef struct {
    char * name;
    bool state;
    unsigned char value;
    uint8_t rgb[3] = {255, 255, 255}; //byte
    char uniqueid[FAUXMO_DEVICE_UNIQUE_ID_LENGTH];
} fauxmoesp_device_t;


static const size_t MAX_PACKET_SIZE = 508;
class udp_mcast_debugComponent : public Component {
  struct sockaddr_in server {};

 public:
  void set_should_multicast() { this->should_multicast_ = true; }
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; };
  void set_tcp_port(uint16_t tcp_port) { this->_tcp_port = tcp_port; }

        unsigned char addDevice(const char * device_name);
        bool renameDevice(unsigned char id, const char * device_name);
        bool renameDevice(const char * old_device_name, const char * new_device_name);
        bool removeDevice(unsigned char id);
        bool removeDevice(const char * device_name);
        char * getDeviceName(unsigned char id, char * buffer, size_t len);
        int getDeviceId(const char * device_name);
        void setDeviceUniqueId(unsigned char id, const char *uniqueid);
        void onSetState(TSetStateCallback fn) { _setStateCallback = fn; }
        void onSetState(TSetStateWithColorCallback fn) { _setStateWithColorCallback = fn; }
        bool setState(unsigned char id, bool state, unsigned char value);
        bool setState(const char * device_name, bool state, unsigned char value);
    //    bool setState(unsigned char id, bool state, unsigned char value, std::byte* rgb);
    //    bool setState(const char * device_name, bool state, unsigned char value, std::byte* rgb);
    //    bool process(AsyncClient *client, bool isGet, String url, String body);
        void enable(bool enable);
        void createServer(bool internal) { _internal = internal; }
        //void setPort(unsigned long tcp_port) { this->_tcp_port = tcp_port; }
        void handle();

private:

 //       AsyncServer * _server;
        bool _enabled = false;
        bool _internal = false;
//        unsigned int _tcp_port = FAUXMO_TCP_PORT;
        std::vector<fauxmoesp_device_t> _devices;
        TSetStateCallback _setStateCallback = NULL;
        TSetStateWithColorCallback _setStateWithColorCallback = NULL;

        std::string _deviceJson(unsigned char id, bool all); 	// all = true means we are listing all devices so use full description template

        void _handleUDP();
     //  void _onUDPData(const IPAddress remoteIP, unsigned int remotePort, void *data, size_t len);
        void _sendUDPResponse();
#ifdef USE_SOCKET_IMPL_LWIP_TCP
        WiFiUDP _udp;
        WiFiEventHandler _handler;
        AsyncClient * _tcpClients[FAUXMO_TCP_MAX_CLIENTS];
        void _onTCPClient(AsyncClient *client);
        bool _onTCPData(AsyncClient *client, void *data, size_t len);
        bool _onTCPRequest(AsyncClient *client, bool isGet, String url, String body);
        bool _onTCPDescription(AsyncClient *client, String url, String body);
        bool _onTCPList(AsyncClient *client, String url, String body);
        bool _onTCPControl(AsyncClient *client, String url, String body);
        void _sendTCPResponse(AsyncClient *client, const char * code, char * body, const char * mime);
#endif
 //       String _byte2hex(uint8_t zahl);
 //       String _makeMD5(String text);
 //       int::byte* _hs2rgb(uint16_t hue, uint8_t sat);
 //       int::byte* _ct2rgb(uint16_t ct);
         

 protected:
  
  uint16_t multicast_port_{1900};
  bool should_multicast_{};
  //bool should_broadcast_{};
  //bool should_listen_{};
  //CallbackManager<void(std::span<const uint8_t>)> packet_listeners_{};
   uint16_t _tcp_port;

#if defined(USE_SOCKET_IMPL_BSD_SOCKETS) || defined(USE_SOCKET_IMPL_LWIP_SOCKETS)
  //std::unique_ptr<socket::Socket> broadcast_socket_ = nullptr;
  //std::unique_ptr<socket::Socket> listen_socket_ = nullptr;
  std::unique_ptr<socket::Socket> multicast_socket_ = nullptr;
  //std::vector<struct sockaddr> sockaddrs_{};
#endif
#ifdef USE_SOCKET_IMPL_LWIP_TCP
  std::vector<IPAddress> ipaddrs_{};
  WiFiUDP udp_client_{};
#endif
  //FixedVector<const char *> addresses_{};

  //optional<network::IPAddress> listen_address_{"239.255.255.250"};
};

}  // namespace esphome::udp
#endif
