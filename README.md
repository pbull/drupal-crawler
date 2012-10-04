drupal-crawler
==============

<pre>
boost-crawl
Crawl updated content on the site so the Boost module can generate a static cache.

 Options:
 --alias-only                              Only follow path aliases and not the node/[NID] paths.                                                   
 --all                                     Initiate a crawl of all content on the site, regardless of updated date and ignoring other options.      
 --days                                    Crawl all content updated in the last n days.                                                            
 --flush                                   Delete all static files from Boost cache prior to initializing crawler.                                  
 --hours                                   Crawl all content updated in the last n hours.                                                           
 --list                                    Output a list of pages that are being crawled. Optionally specify fields to output as --list=type,id,url 
 --simulate                                Simulate crawling of site but do not make requests to the server.                                        
 --timestamp                               Crawl all content updated since timestamp n.


Aliases: crawl


boost-flush
Delete all static files from Boost cache

Aliases: flush
</pre>
