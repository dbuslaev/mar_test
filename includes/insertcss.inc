<?php
/*

Extension of core "css charts" class used for saving data based on raw css file data.
It also saves the css file itself on the drive.

*/

class InsertCss extends DataCss {

  const CODE_EXT_ERROR = -3;
  const CODE_FILE_MISSING = -4;
  const CODE_RESPONSE_EMPTY = -5;

  protected $record_id;


  public function InsertCss($filename, $css_data) {
    if ($this->checkExt($filename)) {
      if (strlen($css_data)>0) {
        $this->localJsonUnlinkAll();

        $market_css = new MarketCss($css_data);
        $css_chart = $market_css->chart();

        $record = array(
          self::TABLE_FILENAME => $filename,
          self::TABLE_CHART => $css_chart
        );
        $this->insert($record);

        if ($this->success()) {

          if (count($this->response) > 0) {
            $this->record_id = (string)$this->response[0][self::TABLE_ID];

            $local_filename = $this->localJsonFilenameById($this->record_id);
            $this->saveLocallyJson($local_filename);

            $this->saveLocallyCss($css_data);

          }else $this->status_code = self::CODE_RESPONSE_EMPTY;
        }
      }else $this->status_code = self::CODE_FILE_MISSING;
    }else $this->status_code = self::CODE_EXT_ERROR;
  }


  protected function checkExt($css_file) {
    $ext = pathinfo($css_file, PATHINFO_EXTENSION);
    return strtolower($ext) == 'css' ? true : false;
  }


  protected function localCssFilename() {
    return '/mnt/tmp/'.$this->record_id.'.css';
  }


  protected function saveLocallyCss($css_data) {
    $local_filename = $this->localCssFilename();
    file_put_contents($local_filename, $css_data);
  }


  protected static function printError($status_code) {
    $error = parent::printError($status_code);
    if (strlen($error) == 0) {
      $errors = array(
        self::CODE_EXT_ERROR => "Only CSS files are allowed",
        self::CODE_FILE_MISSING => "File not found",
        self::CODE_RESPONSE_EMPTY => "Insert response is empty"
      );
      $error=$errors[$status_code];
    }
    return $error;
  }


}