import subprocess

# YouTube RTMP URL and Stream Key
youtube_rtmp_url = "rtmp://b.rtmp.youtube.com/live2?backup=1"
youtube_stream_key = "0cem-9ca7-u5rv-hm0v-3qv8"  # Replace with your YouTube stream key

# Function to start the libcamera-vid and ffmpeg processes for streaming to YouTube Live
def start_libcamera_ffmpeg():
    libcamera_command = [
        'libcamera-vid',
        '-t', '0',                    # Run indefinitely
        '--inline',                   # Inline headers needed for RTMP
        '--width', '1280',            # Set resolution to 720p
        '--height', '720',
        '--framerate', '30',          # 30 FPS for YouTube compatibility
        '--codec', 'h264',            # Use H.264 codec (compatible with YouTube)
        '-o', '-',                    # Output to stdout for piping
        '--nopreview'                 # Disable preview to save resources
    ]
    ffmpeg_command = [
        'ffmpeg',
        '-loglevel', 'debug',         # Enable debug logging
        '-f', 'h264',                 # Specify input format as H.264
        '-i', '-',                    # Input from stdin
        '-f', 'lavfi',                # Generate silent audio if needed
        '-i', 'anullsrc=channel_layout=stereo:sample_rate=44100',
        '-vf', 'fps=30',              # Set frame rate to 30 FPS
        '-vsync', 'cfr',              # Constant frame rate
        '-vcodec', 'libx264',         # H.264 encoding
        '-preset', 'veryfast',        # Use veryfast preset for encoding speed
        '-b:v', '1000k',              # Lowered bitrate to 1000 kbps
        '-maxrate', '1000k',          # Max bitrate
        '-bufsize', '3000k',          # Moderate buffer size for stability
        '-g', '60',                   # Keyframe interval of 2 seconds (30 FPS x 2)
        '-acodec', 'aac',             # AAC codec for audio
        '-b:a', '128k',               # Audio bitrate
        '-ar', '44100',               # Audio sample rate
        '-f', 'flv',                  # Format for RTMP streaming
        f"{youtube_rtmp_url}/{youtube_stream_key}"  # Complete RTMP URL for YouTube
    ]

    # Start the libcamera-vid and ffmpeg processes
    libcamera_process = subprocess.Popen(libcamera_command, stdout=subprocess.PIPE)
    ffmpeg_process = subprocess.Popen(ffmpeg_command, stdin=libcamera_process.stdout, stderr=subprocess.PIPE)

    return libcamera_process, ffmpeg_process

# Start streaming to YouTube Live
libcamera_process, ffmpeg_process = start_libcamera_ffmpeg()

try:
    print("Streaming video to YouTube Live at 720p and 30 FPS. Press Ctrl+C to stop.")
    while True:
        # Print ffmpeg debug output to check for errors
        ffmpeg_output = ffmpeg_process.stderr.readline().decode('utf-8')
        if ffmpeg_output:
            print(ffmpeg_output.strip())

except KeyboardInterrupt:
    print("Streaming stopped.")

finally:
    # Cleanup processes
    if ffmpeg_process.stdin:
        ffmpeg_process.stdin.close()
    ffmpeg_process.wait()
    if libcamera_process.stdout:
        libcamera_process.stdout.close()
    libcamera_process.terminate()
    print("Cleanup complete.")
