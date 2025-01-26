#!/bin/env python3

import markdown

HEADER = '''<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Balloons - prick 'n' puzzle (documentation)</title>
    <meta name="Author" content="Roland Lagarden">
    <meta name="Description" content="Balloons - prick 'n' puzzle">
    <meta name="Keywords" content="Balloons, game, linux">
    <style>
      body { font-face: Arial, sans-serif }
      a, a:link, a:visited { color: #0000ff; text-decoration: none }
      a:active { color: #0096ff }
      a:hover { color: #0096ff; text-decoration: underline }
      hr { color: #990000 }
      kbd { border: 1px ridge #cccccc; padding: 0 2px; box-shadow: 2px 2px #cccccc }
    </style>
  </head>
  <body>
'''

FOOTER = '''
  </body>
</html>
'''

# load
with open("DOCUMENTATION.md", "r") as input_file:
	text = input_file.read()

# markdownify
html = markdown.markdown(text,
	extensions=['toc', 'pymdownx.keys', 'pymdownx.b64'],
	output_format ='html')

# save
with open("index.html", "w") as output_file:
	output_file.write(HEADER)
	output_file.write(html)
	output_file.write(FOOTER)
