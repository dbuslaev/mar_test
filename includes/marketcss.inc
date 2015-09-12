<?php
/*

Creates custom associative array out of parsed css.
Keeps track of the count of each element.

*/

class MarketCss extends ExploreCss {

  var $css_body;
  var $css_assoc = false;

  function MarketCss($css_data) {
    $this->css_body = $css_data;
  }


  public function chart() {
    if (!$this->css_assoc) {
      $css_exploded = $this->explode($this->css_body);
      $selectors = 0;
      $properties = 0;
      foreach ($css_exploded as $values) {
        $selectors++;
        foreach ($values as $prop_name => $value) {
          if(strlen($prop_name) > 0 && strlen($value) > 0){
            $properties++;
            $this->css_assoc[$prop_name][$value]++;
          }
        }
      }
      $this->css_assoc['file']['selector'] = $selectors;
      $this->css_assoc['file']['property'] = $properties;
    }
    return $this->css_assoc;
  }


}
