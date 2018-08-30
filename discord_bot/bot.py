# AIR Polling Bot
# @Jeroen Donkers
# @2018

import os			# finding home directory
import sys			# command line arguments
import asyncio		# async, await
import opuslib 		# broadcasting on voice channel
import discord		# discord bot API
from discord.ext import commands
from discord.ext.commands import Bot

def enable_debugging_flag():
	if len(sys.argv) == 1:
		return False
	return sys.argv[1] == "-d"

def read_file(txtfile):
	file = open(os.path.join(os.path.expanduser('~'),"airadio/" + txtfile), "r")
	return file.read()

def write_file(txtfile, txt):
	file = open(os.path.join(os.path.expanduser('~'),"airadio/" + txtfile), "w")
	file.write(str(txt))

bot = commands.Bot(command_prefix='#')
voice_channel = discord.Object('484765885215670273')	# voice channel ID (for stream audio)
vote_channel = discord.Object('484387225027477507')		# vote channel ID (for txt feedback)

song_title = read_file('currentsong.txt')
stream_url = read_file('streamurl.txt')
bot_key = read_file('botkey.txt')

debugging = enable_debugging_flag()
users_already_voted = []
dislikes = 0
likes = 0

# -------- BOT SETUP --------

@bot.event
async def on_ready():
	print("Starting up " + bot.user.name + "...")
	updatestatus()
	await clear_messages()
	await initial_message()
	if not debugging:
		await stream_in_voice_channel()
	print(bot.user.name + " is up and running...")

# -------- BOT COMMANDS --------

# '#vote D|L'
#	saves likes or dislikes of $song_title
@bot.command(pass_context=True)
async def vote(ctx, msg):
	await bot.delete_message(ctx.message)
	if msg != "L" and msg != "D":
		await bot.say("'" + msg + "' is not a valid vote. Please only use 'L' or 'D' to vote.", delete_after=8)
		return
	user = ctx.message.author
	userVote = getUserVote(user.id)
	if user.id in userVote and userVote[1] == msg:
		await bot.say(ctx.message.author.name + " already voted!", delete_after=8)
	else:
		for t in users_already_voted:
			if user.id in t:					# if user has already voted,
				changevote(t[1], -1)			# remove previous user vote
				users_already_voted.remove(t)
		changevote(msg, 1)
		users_already_voted.append((user.id, msg)) # add new user and vote to the list
		updatestatus()
		await bot.say(user.name + " voted " + msg)

# '#status'
#	displays the amount of likes and dislikes for song_title
@bot.command(pass_context=True)
async def status(ctx):
	await bot.delete_message(ctx.message)
	embed = discord.Embed(title=song_title, description="Amount of votes for this song", color=0x00ff00)
	embed.add_field(name="Likes", value=likes, inline=True)
	embed.add_field(name="Dislikes", value=dislikes, inline=True)
	embed.set_footer(text="type '#vote L' to like the song, say '#vote D' to dislike it!")
	await bot.say(embed=embed)

# '#paasei'
#	easter egg that changes song name to username that found the easter egg
@bot.command(pass_context=True)
async def paasei(ctx):
	await bot.delete_message(ctx.message)
	await bot.say("How nice! You have found my easter egg! You can keep it as a gift.", delete_after=6)
	await asyncio.sleep(6)
	change_song_name(ctx.message.author.name)


# -------- OTHER FUNCTIONS --------

# streams audio of twitch stream in voice_channel of discord
async def stream_in_voice_channel():
	vc = await bot.join_voice_channel(voice_channel)
	player = await vc.create_ytdl_player(stream_url)
	player.start()
	await asyncio.sleep(6)
	if not vc.is_playing():
		await vc.disconnect()

# updates likes and dislikes files
def updatestatus():
	write_file('status_likes.txt', likes)
	write_file('status_dislikes.txt', dislikes)

# changes the name of the song currently playing
def change_song_name(username):
	global song_title
	song_title = username
	write_file('currentsong', song_title)
	clear_messages()

# sends an embedded message about the status of the stream
async def initial_message():
	await bot.send_message(vote_channel, "New song vote is starting...")
	embed=discord.Embed(title="AIRadio is now open for votes!", description="type '#vote L' to like the song or '#vote D' to dislike it!")
	embed.add_field(name="Now Playing:", value=song_title, inline=True)
	embed.set_footer(text="type '#status' to see the amount of likes and dislikes")
	await bot.send_message(vote_channel, embed=embed)
	bot.pin_message(embed)

# adds amnt of votes
def changevote(v, amnt):
	global likes
	global dislikes
	if v == "L":
		likes += amnt
	elif v == "D":
		dislikes += amnt

# finds a user u in users_already_voted with its corresponding vote
def getUserVote(u):
	for t in users_already_voted:
		if u in t:
			return t
	return ("-", "-")

# clears all messages in the vote_channel of discord
async def clear_messages():
	mgs = []
	async for x in bot.logs_from(vote_channel):
		mgs.append(x)

	amnt = len(mgs)
	while amnt > 1:
		amnt -= 100	# bot can delete a maximum of 100 messages
		await bot.delete_messages(mgs)

# -------- RUNNING BOT --------
bot.run(bot_key)