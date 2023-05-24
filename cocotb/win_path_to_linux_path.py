import re
import argparse


parser = argparse.ArgumentParser()
parser.add_argument('-s', '--source', default=R'C:\1.txt',
                    help='source path to be converted')
args = parser.parse_args()

def main():
    print( "/" + "/".join(filter(None, re.split('[:\\\\/]+', args.source))))

if __name__ == '__main__':
    main()