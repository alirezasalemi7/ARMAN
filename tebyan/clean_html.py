from bs4 import BeautifulSoup
import bs4
import glob
import tqdm
import pandas as pd
import pymongo
import langdetect

def clean_and_save(file_name):
    with open(file_name) as file:
        soup = BeautifulSoup(file.read(),'html5lib')
        summary_element = soup.find(attrs={"class":"ArticleSummary"})
        summary_text = None
        if summary_element:
            summary_text = summary_element.__dict__['contents'][0]

        article_element = soup.find(attrs={"class":"Article"})
        if not article_element:
            article_element = soup.find(attrs={"class":"NewArticleContent"})

        article_text = None
        if article_element:
            article_text = ""
            article_contents = article_element.__dict__['contents']
            for content in article_contents:
                if type(content) == bs4.element.Tag:
                    if content.name != "div":
                        article_text += content.text
                    if content.name == "br":
                        article_text+= "\n"
                if type(content) == bs4.element.NavigableString:
                    article_text += content
    try:
        if article_text and summary_text and langdetect.detect(article_text) == "fa":
            return article_text,summary_text
    except:
        pass
    return None,None


myclient = pymongo.MongoClient("mongodb://localhost:27017/")
mydb = myclient["tebyan_database"]
mycol = mydb["summary_dataset"]

files = glob.glob("./download/*.html")
total = 0
total_correct = 0
# articles = []
# summaries = []
for file in tqdm.tqdm(files):
    article,summary = clean_and_save(file)
    if article and summary:
        instace_summary = { "article": article, "summary": summary }
        x = mycol.insert_one(instace_summary)
        # articles.append(article)
        # summaries.append(summary)
        total_correct += 1
    total += 1

# dataframe = pd.DataFrame(
#     {
#         "article" : articles,
#         "summary" : summaries
#     }
# )

# dataframe.to_csv("data.csv")
print(total,total_correct)