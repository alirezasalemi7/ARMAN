# Tebyan, a Persian Lifestyle website

    The Tebyan cultural institute, which is affiliated to the organization "Sazman-e Tablighat-e Eslami", is one of the biggest and best known cultural institutes in Iran, and has cooperated with other cultural institutes in different fields for supporting cultural festivals and broadcasting their activities in the media. The activities of the institute's take place not only in Tehran, but also in the provincial centers, and 1,600,000 users visit its website each day. The Iranian deputy minister supported its activities for sport and youth on the website tebyan.net in Tehran.

## crawling webpages

We have used simple GET requests to download pages. Tebyan website URLs are in the form of `article.tebyan.net/[article-number]`, so we started from the article with id 379900 and finished with 479900. In order to download these pages, `crawl.py` should be run.

## cleaning crawled files

The procedure that we used for cleaning crawled pages is in `clean_html.py` script. By executing this script, crawled HTML files will be cleaned and stored in the MongoDB database.

