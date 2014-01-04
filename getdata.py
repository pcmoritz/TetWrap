from urllib import urlopen
from StringIO import StringIO
from zipfile import ZipFile

url = urlopen('http://www.holmes3d.net/graphics/offfiles/Letters.zip')
f = ZipFile(StringIO(url.read()))
content = f.open("A.off").read().replace('\r\n', '\n')
localfile = open("A.off", 'w')
localfile.write(content)
localfile.close()

