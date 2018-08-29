#AIR Polling Bot by Jeroen Donkers c2018

import os
import asyncio
import discord
from discord.ext import commands
from discord.ext.commands import Bot

bot = commands.Bot(command_prefix='#')
users_already_voted = []
song_title = " "
likes = 0
dislikes = 0

# -------- BOT STARTUP --------

@bot.event
async def on_ready():
	print("Starting up " + bot.user.name + "...")
	updatestatus()	# update likes and dislikes files
	# read song title
	file = open(os.path.join(os.path.expanduser('~'),'AIRADIO_currentsong.txt'), "r")
	song_title = file.read()
	await clear_messages()
	# send initial startup message
	await bot.send_message(discord.Object('484387225027477507'), "New song vote is starting...")
	embed=discord.Embed(title="AIRadio is now open for votes!", description="type '#vote L' to like the song or '#vote D' to dislike it!")
	embed.add_field(name="Now Playing:", value=song_title, inline=True)
	embed.set_footer(text="type '#status' to see the amount of likes and dislikes")
	await bot.send_message(discord.Object('484387225027477507'), embed=embed)
	bot.pin_message(embed)
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

# -------- OTHER FUNCTIONS --------

def updatestatus():
	file = open(os.path.join(os.path.expanduser('~'),'AIRADIO_status_likes.txt'), "w")
	file.write(str(likes))
	file = open(os.path.join(os.path.expanduser('~'),'AIRADIO_status_dislikes.txt'), "w")
	file.write(str(dislikes))

def changevote(v, amnt):
	global likes
	global dislikes
	if v == "L":
		likes += amnt
	elif v == "D":
		dislikes += amnt

def getUserVote(u):
	for t in users_already_voted:
		if u in t:
			return t
	return ("-", "-")

async def clear_messages():
	mgs = []
	async for x in bot.logs_from(discord.Object('484387225027477507')):
		mgs.append(x)
	await bot.delete_messages(mgs)

# -------- RUNNING BOT --------
bot.run("NDg0Mzc2Njk5OTI5MzYyNDMz.DmhPdQ.iQlmyNQCpRB3fcQIVTdqjJrDn4o")