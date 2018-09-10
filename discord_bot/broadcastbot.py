# AIR Broadcast Bot
# @Jeroen Donkers
# @2018

import os			# finding home directory
import asyncio		# async, await
import opuslib 		# broadcasting on voice channel
import discord		# discord bot API
from discord.ext import commands
from discord.ext.commands import Bot

def read_file(txtfile):
	file = open(os.path.join(os.path.expanduser('~'),"airadio/" + txtfile), "r")
	return file.read()

def write_file(txtfile, txt):
	file = open(os.path.join(os.path.expanduser('~'),"airadio/" + txtfile), "w")
	file.write(str(txt))

bot = commands.Bot(command_prefix='&')
voice_channel = discord.Object('484765885215670273')	# voice channel ID (for stream audio)
stream_url = read_file('streamurl.txt')
bot_key = read_file('broadcastbotkey.txt')

@bot.event
async def on_ready():
	print("Starting up " + bot.user.name + "...")
	await stream_in_voice_channel()
	print(bot.user.name + " is up and running...")

# streams audio of twitch stream in voice_channel of discord
async def stream_in_voice_channel():
	vc = await bot.join_voice_channel(voice_channel)
	player = await vc.create_ytdl_player(stream_url)
	player.start()
	await asyncio.sleep(6)
	if not vc.is_playing():
		await vc.disconnect()

# -------- RUNNING BOT --------
bot.run(bot_key)