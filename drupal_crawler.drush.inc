<?php

/**
 * @file
 * Drush integration for crawling a site that uses the Boost module to
 * implement a static file cache.
 */

/**
 * Implementation of hook_drush_help().
 */
function drupal_crawler_drush_help($section) {
  switch ($section) {
    case 'meta:import-redirects:title':
      return dt('Drupal Boost crawler commands');
    case 'meta:import-redirects:summary':
      return dt('Crawl updated content on the site so the Boost module can generate a static cache.');
  }
}

/**
 * Implementation of hook_drush_command().
 */
function drupal_crawler_drush_command() {
  $items['boost-crawl'] = array(
    'description' => 'Crawl updated content on the site so the Boost module can generate a static cache.',
    'drupal dependencies' => array('boost'),
    'options' => array(
      'all' => 'Initiate a crawl of all content on the site, regardless of updated date and ignoring other options.',
      'days' => 'Crawl all content updated in the last n days.',
      'hours' => 'Crawl all content updated in the last n hours.',
      'timestamp' => 'Crawl all content updated since timestamp n.',
      'flush' => 'Delete all static files from Boost cache prior to initializing crawler.',
      'list' => 'Output a list of URLs that are being crawled.',
      'simulate' => 'Simulate crawling of site but do not make requests to the server.',
    ),
    'aliases' => array('crawl'),
  );
  $items['boost-flush'] = array(
    'description' => 'Delete all static files from Boost cache',
    'drupal dependencies' => array('boost'),
    'aliases' => array('flush'),
  );
  
  return $items;
}

/**
 * Command callback. Crawl a Drupal site.
 */
function drush_drupal_crawler_boost_crawl($file = NULL) {
  if (drush_drupal_major_version() != 7) {
    drush_set_error(dt('Drupal Boost crawler requires Drupal 7.'));
    return;
  }
  
  // Set semaphore so simultaneous crawls don't clobber one another
  if (!lock_acquire('drush_drupal_crawler')) {
    drush_set_error(dt('Drupal Boost crawler is already running or cannot acquire a lock.'));
    return;
  }
  
  // Crawler last run
  $drupal_crawler_last = variable_get('drupal_crawler_last', 0);
  //drush_print($drupal_crawler_last);
  
  // Flush Boost cache?
  if (drush_get_option('flush') && !drush_get_option('simulate')) {
    drupal_crawler_flush_boost_cache();
  }
  
  // Determine time window for new/updated content
  $timestamp_changed = $drupal_crawler_last;
  if (drush_get_option('all')) {
    $timestamp_changed = 0;
    if (drush_get_context('DRUSH_VERBOSE')) {
      drush_print(dt('Crawling all published nodes.'));
    }
  }
  else {
    if ($timestamp = drush_get_option('timestamp')) {
      $timestamp_changed = $timestamp;
    }
    elseif ($days = drush_get_option('days')) {
      $timestamp_changed = REQUEST_TIME - ($days * 60 * 60 * 24);
    }
    elseif ($hours = drush_get_option('hours')) {
      $timestamp_changed = REQUEST_TIME - ($hours * 60* 60);
    }
    if (drush_get_context('DRUSH_VERBOSE')) {
      drush_print(dt('Crawling published nodes updated since: @date',
        array('@date' => format_date($timestamp_changed, 'custom', 'r'))));
    }
  }
  
  // List of paths to crawl
  $crawl_list = array();
  
  // Initialize crawler, enable Boost
  $commands = drush_get_commands();
  $path = $commands['boost-crawl']['path'];
  variable_set('boost_enabled_text/html', 1);
  variable_set('boost_enabled_text/javascript', 1);
  
  // Front page crawler
  include_once($path . '/includes/drupal_crawler.front-page.inc');
  
  // Node crawler
  include_once($path . '/includes/drupal_crawler.node.inc');
  
  // Crawl
  drupal_crawler_crawl($crawl_list);
  
  // Clean up
  if (!drush_get_option('simulate')) {
    variable_set('drupal_crawler_last', REQUEST_TIME);
  }
  variable_set('boost_enabled_text/html', 0);
  variable_set('boost_enabled_text/javascript', 0);
  lock_release('drush_drupal_crawler');
}

/**
 * Command callback. Delete all static files from Boost cache.
 */
function drush_drupal_crawler_boost_flush() {
  if (drush_confirm('All static files will be deleted from Boost cache. Are you sure you want to continue?')) {
    drupal_crawler_flush_boost_cache();
  }
}

/**
 * Crawl an array of paths.
 */
function drupal_crawler_crawl($crawl_list = array()) {
  foreach ($crawl_list as $path => $path_info) {
    // Print URL to screen
    if (drush_get_context('DRUSH_VERBOSE')) {
      drush_print(t('Crawling @path', array('@path' => $path_info['url'])));
    }
    // Output a list of raw URLs
    if (drush_get_option('list')) {
      drush_print($path_info['url']);
    }
    // Currently just using drupal_http_request().
    if (!drush_get_option('simulate')) {
      $r = drupal_http_request($path_info['url']);
    }
  }
}

/**
 * Delete all static files from Boost cache.
 */
function drupal_crawler_flush_boost_cache() {
  drush_print('Deleting all static files from Boost cache.');
  boost_flush_caches();
}
