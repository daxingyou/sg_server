<?php
class mail_recv_limit extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
  }
  function level()
  {
    return $this->_get_value("1");
  }
  function set_level($value)
  {
    return $this->_set_value("1", $value);
  }
  function regist_type()
  {
    return $this->_get_value("2");
  }
  function set_regist_type($value)
  {
    return $this->_set_value("2", $value);
  }
}
class global_mail_single extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBString";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
    $this->fields["3"] = "PBString";
    $this->values["3"] = "";
    $this->fields["4"] = "PBString";
    $this->values["4"] = "";
    $this->fields["5"] = "PBString";
    $this->values["5"] = "";
    $this->fields["6"] = "PBInt";
    $this->values["6"] = "";
    $this->fields["7"] = "PBInt";
    $this->values["7"] = "";
    $this->fields["8"] = "PBInt";
    $this->values["8"] = "";
    $this->fields["9"] = "PBInt";
    $this->values["9"] = "";
    $this->fields["10"] = "PBInt";
    $this->values["10"] = "";
    $this->fields["11"] = "PBString";
    $this->values["11"] = array();
    $this->fields["12"] = "PBInt";
    $this->values["12"] = "";
    $this->fields["13"] = "PBString";
    $this->values["13"] = "";
    $this->fields["14"] = "mail_recv_limit";
    $this->values["14"] = "";
  }
  function uid()
  {
    return $this->_get_value("1");
  }
  function set_uid($value)
  {
    return $this->_set_value("1", $value);
  }
  function type()
  {
    return $this->_get_value("2");
  }
  function set_type($value)
  {
    return $this->_set_value("2", $value);
  }
  function title()
  {
    return $this->_get_value("3");
  }
  function set_title($value)
  {
    return $this->_set_value("3", $value);
  }
  function content()
  {
    return $this->_get_value("4");
  }
  function set_content($value)
  {
    return $this->_set_value("4", $value);
  }
  function send_name()
  {
    return $this->_get_value("5");
  }
  function set_send_name($value)
  {
    return $this->_set_value("5", $value);
  }
  function send_time()
  {
    return $this->_get_value("6");
  }
  function set_send_time($value)
  {
    return $this->_set_value("6", $value);
  }
  function over_time()
  {
    return $this->_get_value("7");
  }
  function set_over_time($value)
  {
    return $this->_set_value("7", $value);
  }
  function send_type()
  {
    return $this->_get_value("8");
  }
  function set_send_type($value)
  {
    return $this->_set_value("8", $value);
  }
  function send_hour()
  {
    return $this->_get_value("9");
  }
  function set_send_hour($value)
  {
    return $this->_set_value("9", $value);
  }
  function recv_type()
  {
    return $this->_get_value("10");
  }
  function set_recv_type($value)
  {
    return $this->_set_value("10", $value);
  }
  function recv_did_list($offset)
  {
    $v = $this->_get_arr_value("11", $offset);
    return $v->get_value();
  }
  function append_recv_did_list($value)
  {
    $v = $this->_add_arr_value("11");
    $v->set_value($value);
  }
  function set_recv_did_list($index, $value)
  {
    $v = new $this->fields["11"]();
    $v->set_value($value);
    $this->_set_arr_value("11", $index, $v);
  }
  function remove_last_recv_did_list()
  {
    $this->_remove_last_arr_value("11");
  }
  function recv_did_list_size()
  {
    return $this->_get_arr_size("11");
  }
  function client_valid_time()
  {
    return $this->_get_value("12");
  }
  function set_client_valid_time($value)
  {
    return $this->_set_value("12", $value);
  }
  function items()
  {
    return $this->_get_value("13");
  }
  function set_items($value)
  {
    return $this->_set_value("13", $value);
  }
  function limit()
  {
    return $this->_get_value("14");
  }
  function set_limit($value)
  {
    return $this->_set_value("14", $value);
  }
}
class tm_send_mail_request extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function reply_code()
  {
    return $this->_get_value("1");
  }
  function set_reply_code($value)
  {
    return $this->_set_value("1", $value);
  }
}
class mt_send_mail_reply extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function reply_code()
  {
    return $this->_get_value("1");
  }
  function set_reply_code($value)
  {
    return $this->_set_value("1", $value);
  }
}
?>