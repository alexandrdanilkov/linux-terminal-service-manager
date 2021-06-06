# linux-terminal-service-manager
Linux Terminal Service Manager (LTSM) is a set of service programs that allows remote computers to connect to a Linux operating system computer using a remote terminal session (over VNC or RDP)

# Linux Only!
main dependencies:  
  - systemd, sd-bus  
  - [sdbus-cpp](https://github.com/Kistler-Group/sdbus-cpp)  
  - [json parser](https://github.com/zserge/jsmn)
  - XCB system libs  

# Demo access
```
vncviewer 62.109.24.208
logins: demo1, demo2, demo3, demo4
pass: demo
```
this is a single core system, sorry...

# Схема взаимодействия компонентов
![ltsm_diagram](https://user-images.githubusercontent.com/8620726/118247282-884e7480-b492-11eb-92a8-d8db95656eee.png)  
реализованы следующие компоненты:

# LTSM_service
основная служба, менеджер dbus ltsm.service.manager, получает команды от LTSM_connector, запускает login и users сессии на базе Xvfb (Лицензия GPLv3)  
see also: https://github.com/AndreyBarmaley/linux-terminal-service-manager/wiki/LTSM-service  

# LTSM_connector
является только обработчиком сетевого протокола VNC и RDP, а основной сетевой частью занимается служебный xinetd/(systemd sockets), также он является клиентом dbus ltsm.manager.service, подключается к Xvfb через механизм shared memory (Лицензия на коннектор Affero GPLv3)  
see also: https://github.com/AndreyBarmaley/linux-terminal-service-manager/wiki/LTSM-connector  

# LTSM_helper
![ltsm_helper](https://user-images.githubusercontent.com/8620726/120913449-cfd9b200-c686-11eb-947e-dc0f20e6eda7.png)  
графическая утилита входа в систему, является клиентом dbus ltsm.manager.service (Лицензия GPLv3)  
see also: https://github.com/AndreyBarmaley/linux-terminal-service-manager/wiki/LTSM-config-(full-description)  

# LTSM_sessions
![ltsm_session](https://user-images.githubusercontent.com/8620726/119793454-23e5d900-bec6-11eb-9978-ee31f44360ae.png)  
графическая утилита управления сессиями пользователей, является клиентом dbus ltsm.manager.service (Лицензия GPLv3)  
see also: https://github.com/AndreyBarmaley/linux-terminal-service-manager/wiki/LTSM-administrator  

## Donate
<a href="https://paypal.me/andreyafletdinov/"><img src="blue.svg" height="40"></a>  
If you enjoyed this project — or just feeling generous, consider buying me a beer. Cheers!
