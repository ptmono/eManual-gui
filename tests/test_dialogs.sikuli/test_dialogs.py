import os

week_img = "image1.png"
create_img = "image2.png"
group_img = "image3.png"
create_name_img = "image4.png"
ok_img = "image5.png"


#mmd = Settings.MoveMouseDelay   # default 0.5
#ddm = Settings.WaitScanRate     # default 3.0


openApp("build/Test")
wait(week_img, 30)

def createElement(name):
    rightClick(week_img)
    wait(create_img, 4)
    click(create_img)
    wait(1)
    type(name)
    wait(2)
    type("o", KEY_ALT)


createElement("aa")
wait(2)
createElement("bb")

