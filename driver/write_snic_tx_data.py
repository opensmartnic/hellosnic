import packnet

src = ["192.168.11.9", 0, "06:05:04:03:02:da"]   # defining ip, port and mac
dst = ["192.168.11.10", 0, "ff:ff:ff:ff:ff:ff"]

package = packnet.Packager()        # creating packager object
package.fill( packnet.ARP.Header )  # tell packager to make use of the ARP class
package.src = src                   # set the source and destination addresses
package.dst = dst

print( package.packet )   # print the created packet in bytes

with open('/dev/scull0', 'wb') as fp:
    fp.seek(0, 0)
    fp.write(package.packet) 

print('finish')
