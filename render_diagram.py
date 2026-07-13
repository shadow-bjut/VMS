import urllib.request

puml_path = r'C:\Users\shadow\Desktop\claude code\c++keshe\project\VMS\VMS_class_diagram.puml'
png_path = r'C:\Users\shadow\Desktop\claude code\c++keshe\project\VMS\VMS_class_diagram.png'

with open(puml_path, 'rb') as f:
    data = f.read()

req = urllib.request.Request(
    'https://kroki.io/plantuml/png',
    data=data,
    headers={'Content-Type': 'text/plain'}
)

resp = urllib.request.urlopen(req, timeout=30)
with open(png_path, 'wb') as f:
    f.write(resp.read())

print('OK - diagram saved to VMS_class_diagram.png')
