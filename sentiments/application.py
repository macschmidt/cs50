from flask import Flask, redirect, render_template, request, url_for

import helpers
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name, count = 10)
    
    # instantiate analyzer
    analyzer = Analyzer()
    
    positiveTweets = 0;
    negativeTweets = 0;
    neutralTweets = 0;
    positive, negative, neutral = 0, 0, 0
    
    if tweets:
        for currentTweet in tweets:
            score = analyzer.analyze(currentTweet)
            if score > 0.0:
                positiveTweets += 1
            elif score < 0.0:
                negativeTweets += 1
            else:
                neutralTweets += 1
            
            denom = positiveTweets + negativeTweets + neutralTweets
            positive = (positiveTweets / denom) * 100
            negative = (negativeTweets / denom) * 100
            neutral = (neutralTweets / denom) * 100

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
