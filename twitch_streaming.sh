#! /bin/bash

# streaming on Ubuntu via ffmpeg.
# see http://ubuntuguide.org/wiki/Screencasts for full documentation
# see http://www.thegameengine.org/miscellaneous/streaming-twitch-tv-ubuntu/
# for instructions on how to use this gist

if [ ! -f ./twitch_key ]; then
    echo "Error: Could not find file: ~/.twitch_key"
    echo "Please create this file and copy past your stream key into it. Open this script for more details."
    exit 1;
fi

# input resolution, currently fullscreen.
# you can set it manually in the format "WIDTHxHEIGHT" instead.
INRES=$(xwininfo -root | awk '/geometry/ {print $2}'i)

# output resolution.
# keep the aspect ratio the same or your stream will not fill the display.
OUTRES="1600x900"

# input audio. You can use "/dev/dsp" for your primary audio input.
INAUD="pulse"

# target fps
FPS="60"

# video preset quality level.
# more FFMPEG presets avaiable in /usr/share/ffmpeg
# According to trac.ffmpeg.org/wiki/x264EncodingGuide the presets are ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow, placebo
QUAL="fast"

# stream key. You can set this manually, or reference it from a hidden file like what is done here.
STREAM_KEY=$(cat ./twitch_key)

# stream url. Note the formats for twitch.tv and justin.tv
# twitch:"rtmp://live.twitch.tv/app/$STREAM_KEY"
# justin:"rtmp://live.justin.tv/app/$STREAM_KEY"
STREAM_URL="rtmp://live.twitch.tv/app/$STREAM_KEY"

ffmpeg \
-f alsa -ac 2 -i "$INAUD" \
-f x11grab -s "$INRES" -r "$FPS" -i :0.0 \
-vcodec libx264 -s "$OUTRES" -vpre "$QUAL" \
-acodec libmp3lame -threads 6 -qscale 5 -b 64KB \
-f flv -ar 44100 "$STREAM_URL"
