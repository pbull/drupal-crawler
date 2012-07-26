drupal-crawler
==============

boost-crawl
Crawl updated content on the site so the Boost module can generate a static cache.

Options:
 --all                                     Initiate a crawl of all content on the site, regardless of updated date and ignoring other options. 
 --days                                    Crawl all content updated in the last n days.                                                       
 --hours                                   Crawl all content updated in the last n hours.                                                      
 --timestamp                               Crawl all content updated since timestamp n.                                                        
 --flush                                   Delete all static files from Boost cache prior to initializing crawler.

Aliases: crawl


boost-flush
Delete all static files from Boost cache

Aliases: flush