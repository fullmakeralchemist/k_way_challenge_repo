from bluepy import btle


def my_main_code(new_value):
    print("new value is: ", new_value)


class MyDelegate(btle.DefaultDelegate):
    def __init__(self, callback):
        self.callback = callback
        btle.DefaultDelegate.__init__(self)
        # ... initialise here

    def handleNotification(self, cHandle, data):
        # Convert data to be easily consumed by callback
        tum = int.from_bytes(data, byteorder='big')
        self.callback(tum)


# Initialisation  -------
p = btle.Peripheral("1a:b4:c0:25:3a:aa")#change MAC adress
p.setDelegate( MyDelegate(my_main_code) )

# Setup to turn notifications on, e.g.
svc = p.getServiceByUUID("81c30e5c-1101-4f7d-a886-de3e90749161")
ch = svc.getCharacteristics("81c30e5c-2101-4f7d-a886-de3e90749161")[0]

setup_data = b"\x01\00"
p.writeCharacteristic(ch.valHandle+1, setup_data)

# Main loop --------

while True:
    if p.waitForNotifications(1.0):
        # handleNotification() was called
        continue

    print("Waiting...")