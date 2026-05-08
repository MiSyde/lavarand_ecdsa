# Lavarand & ECDSA 🌋🫧🔑
A console application written in C, that can take 2 types of input;

1. A file path to an image,
2. A picture taken with the user's camera

After the mode was chosen, the user needs to select which color it wants the image processing algorithm to focus on - meaning: what are the colors of the lava bubbles inside the lavalamp.\
After the image has been processed and the bubbles' binary values are turned into a hash value using sha256, this hash is used in the ECDSA's private key.
> [!NOTE]
> The ECDSA part is still being worked on. This project is not finished yet.
## The colors that are currently available: 🎨
- Red
- Orange
- Yellow
- Green
- Cyan
- Blue
- Purple
- Pink

## How does the camera mode work? ⋆.📷˚
After the user has chosen to give input through camera, they can choose whether they want to take a picture manually or automatically.
If the program detects atleast one camera, it lists all cameras it detected and lets the user choose one of them.
### Automatic mode
The camera takes a picture by itself whenever the first frame is available, then it shuts down.
### Manual mode
The user needs to press `T` inside the LavaCam window, after the first frame is available, then it shuts down.
> [!IMPORTANT]
> It's trivial but I still feel it important to mention; The color needs to be in proper lighting!\
> For example:\
> If you select red detection, but provide an image where it's so dark the red looks brown; of course it won't work with that given mode.
