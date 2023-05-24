import logging
import argparse


# the logging.basicConfig() once set cannot be changed
logging.basicConfig(level = logging.INFO,format = '%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)
logger.setLevel('INFO')

parser = argparse.ArgumentParser()
parser.add_argument('-d', '--device', default= '/dev/scull0', #'tmpfile',#
                    help='')
parser.add_argument('-a', '--address', default='0x04',
                    help='')
parser.add_argument('-v', '--value', default='', #'0x6462620a',
                    help='采用小端序')

args = parser.parse_args()

def main():
    addr = int(args.address, base=16)
    mode = 'r' if args.value == '' else 'w'
    if mode == 'r':
        with open(args.device, 'rb') as fp:
            fp.seek(addr, 0)
            bytes_read = fp.read(4)
            value = int.from_bytes(bytes_read, 'little')
            logger.info('read from {0:#010x}: {1:#010x}'.format(addr, value))
    else:
        value = int(args.value, base=16)
        with open(args.device, 'wb') as fp:
            fp.seek(addr, 0)
            bytes_to_write = value.to_bytes(4, 'little')
            fp.write(bytes_to_write) 
            logger.info('write to {0:#010x}: {1:#010x}'.format(addr, value))


if __name__ == '__main__':
    # logger.info(f'program start, args = {args} ')
    main()
