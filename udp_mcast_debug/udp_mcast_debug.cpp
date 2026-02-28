#include "esphome/core/defines.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/components/network/util.h"
#include "udp_mcast_debug.h"

#define MULTICAST_IPV4_ADDR "239.255.255.250"   //IP multicast
#define MULTICAST_PORT "1900"

static const char *TAG = "multicast-ipv4-SSDP";

namespace esphome::udp_mcast_debug {

void udp_mcast_debugComponent::setup() {
  #if defined(USE_SOCKET_IMPL_BSD_SOCKETS) || defined(USE_SOCKET_IMPL_LWIP_SOCKETS)
  
  //int enable = 1;
 //struct sockaddr_in server {};
 // struct sockaddr saddr {};
  //this->sockaddrs_.push_back(sockaddr_in);
 //   socket::set_sockaddr(&saddr, sizeof(saddr), address, this->multicast_port_);
 //   this->sockaddrs_.push_back(saddr);

  this->multicast_socket_ = socket::socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (this->multicast_socket_ == nullptr) {
      this->status_set_error(LOG_STR("Could not create socket"));
      this->mark_failed();
      return;
    }

  
    server.sin_family = PF_INET;
    server.sin_port = htons(this->multicast_port_);
    server.sin_addr.s_addr = htonl(ESPHOME_INADDR_ANY);
    

  auto err = this->multicast_socket_->bind((struct sockaddr *) &server, sizeof(server));
    if (err != 0) {
      ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
      this->status_set_error(LOG_STR("Unable to bind socket"));
      this->mark_failed();
      return;
    }
     
    err = this->multicast_socket_->setblocking(false);
    if (err < 0) {
      ESP_LOGE(TAG, "Unable to set nonblocking: errno %d", errno);
      this->status_set_error(LOG_STR("Unable to set nonblocking"));
      this->mark_failed();
      return;
    }
    
    err = this->multicast_socket_->setsockopt(IPPROTO_IP, IP_MULTICAST_IF, &server, sizeof(server));
    if (err < 0) {
      this->status_set_warning(LOG_STR("Socket unable to set IP_MULTICAST_IF"));
      return;
     }
    
    struct ip_mreq imreq = {};
      imreq.imr_interface.s_addr = ESPHOME_INADDR_ANY;
      inet_aton(MULTICAST_IPV4_ADDR, &imreq.imr_multiaddr);
      server.sin_addr.s_addr = imreq.imr_multiaddr.s_addr;
        
    err = this->multicast_socket_->setsockopt(IPPROTO_IP, IP_ADD_MEMBERSHIP, &imreq, sizeof(imreq));
      if (err < 0) {
        ESP_LOGE(TAG, "Failed to set IP_ADD_MEMBERSHIP. Error %d", errno);
        this->status_set_error(LOG_STR("Failed to set IP_ADD_MEMBERSHIP"));
        this->mark_failed();
        return;
      }
        
    return;
    
#endif 
  
} // void setup() end

void udp_mcast_debugComponent::loop() {
  
 
    std::array<uint8_t, MAX_PACKET_SIZE> buf;
    for (;;) {
#if defined(USE_SOCKET_IMPL_BSD_SOCKETS) || defined(USE_SOCKET_IMPL_LWIP_SOCKETS)
      auto len = this->multicast_socket_->read(buf.data(), buf.size());
#endif
#ifdef USE_SOCKET_IMPL_LWIP_TCP
      auto len = this->udp_client_.parsePacket();
      if (len > 0)
        len = this->udp_client_.read(buf.data(), buf.size());
#endif
      if (len <= 0)
        break;
      size_t packet_len = static_cast<size_t>(len);
      //std::string xtest = (char *)buf.data();
  //    auto formated_hex = format_hex_pretty(buf.data(), buf.size());
  //    ESP_LOGD("formatted_hex", "Received Bytes: %s", formated_hex.c_str());
          std::string xtest = (char *)buf.data();
                   
                auto formated_hex = format_hex_pretty(buf.data(), buf.size());
                   //ESP_LOGI("ALEXA", "found M-SEARCH");
                   ESP_LOGD("xtest", "Received Message: %s", xtest.c_str());
                   //ESP_LOGD("formatted_hex", "Received Bytes: %s", formated_hex.c_str());
    
    
                  }
             
        
} // loop() end

void udp_mcast_debugComponent::dump_config() {
  ESP_LOGCONFIG(TAG,
                "UDP-Multicast all Datas received on:\n"
                "  IP: %s\n"
                "  Port: %u\n"
                , MULTICAST_IPV4_ADDR, this->multicast_port_);
 } // dump_config() End
}  // namespace esphome::udp_mcast_debug