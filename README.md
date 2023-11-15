# proyecto_tharry_robotica
archivos necesarios para implementar y usar al robot Tharry
1. Tharry es el paquete en python que hay que colocar en src para poder visualizar todo en gazebo(los meshes hay que copiarlos en la carpeta .gazebo, los mismo para my_bot).
2. my_bot es el mismo paquete pero implementado en lenguaje c con cmake y ademas posee la propiedad de visualizar el sensor de ultrasonido.
3. vel_publicador es el paquete que contiene los nodos publicadores que publican el mensaje tipo Twist en el topic /cmd_vel.
4. arduino_code es el codigo que se debe implementar en el arduino.
5. es necesario descargar la aplicación RemoteXY en el celular(se puede conseguir desde la playStore), con dicha app podran conectarse al modulo bluetooth y visualizar la interfaz de control en el celular.(hay una imagen de como se deberia ver en el main de este git)
