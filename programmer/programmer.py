import subprocess
import time

args = ["openocd", "-f", "interface/raspberrypi-swd.cfg", "-f", "target/rp2040.cfg", "-c" "program target.elf verify reset exit"]

while True:

    process = subprocess.Popen(args,
                            stdout=subprocess.PIPE, 
                            stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return_code = process.poll()

    if return_code == 0:
        print("It worked!")
        time.sleep(5)
    else:
        print("Not this time. Will try again.")
        time.sleep(1)
        # print(stderr.decode("UTF-8"))
