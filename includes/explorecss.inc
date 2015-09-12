<?php
/*

This is the core css parsing class.

*/

class ExploreCss {

  var $css_exploded = false;

  protected function explode($css) {
    if (!$this->css_exploded) {
      preg_match_all('/(.+?)\s?\{\s?(.+?)\s?\}/', $css, $matches);
      foreach ($matches[0] as $i => $original) {
        foreach (explode(';', $matches[2][$i]) as $attr) {
          if (strlen(trim($attr)) > 0) { // for missing semicolon on last element, which is legal
            list($name, $value) = explode(':', $attr);
            $this->css_exploded[$matches[1][$i]][trim($name)] = trim($value);
          }
        }
      }
    }
    return $this->css_exploded;
  }


}
