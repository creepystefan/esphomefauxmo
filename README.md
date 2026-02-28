```yaml
external_components:
   - source: github://creepystefan/esphomefauxmo
     refresh: 0s

esp32:
  board: esp32dev
  framework:
    type: esp-idf

logger:
  level: DEBUG

udp_mcast_debug:

```



```yaml
udp:
  listen_address: 239.255.255.250
  port: 1900
  on_receive:
    then:
      - lambda: |-
          if (!data.empty() && data[0] == 0x4D && 
               data[1] == 0x2D && data[2] == 0x53 &&
               data[3] == 0x45 && data[4] == 0x41 &&
               data[5] == 0x52 && data[6] == 0x43 &&
               data[7] == 0x48 && data[8] == 0x20
             ) {
            ESP_LOGI("ALEXA", "alexa search M-SEARCH");
          }
```
```yaml
### for logging all searching datas

udp:
  listen_address: 239.255.255.250
  port: 1900
  on_receive:
    then:
      - logger.log:
          format: "Received %s"
          args: [format_hex_pretty(data).c_str()]
```






 Received 4D.2D.53.45.41.52.43.48.20.2A.20.48.54.54.50.2F.31.2E.31.0D.
          0A.48.4F.53.54.3A.20.32.33.39.2E.32.35.35.2E.32.35.35.2E.32.35.30.3A.31.39.30.30.0D.
          0A.53.54.3A.20.73.73.64.70.3A.61.6C.6C.0D.
          0A.4D.41.4E.3A.20.22.73.73.64.70.3A.64.69.73.63.6F.76.65.72.22.0D.
          0A.4D.58.3A.20.33.0D.0A.0D.0A (94)



4D2D534541524348202A20485454502F312E310D
0A484F53543A203233392E3235352E3235352E3235303A313930300D
0A53543A20737364703A616C6C0D
0A4D414E3A2022737364703A646973636F766572220D
0A4D583A20330D0A0D0A



M-SEARCH * HTTP/1.1
HOST: 239.255.255.250:1900
ST: ssdp:all
MAN: "ssdp:discover"
MX: 3

