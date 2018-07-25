<?php
class MsgID extends PBEnum
{
  const MsgAdmin  = 10000;
  const MsgAdminMail  = 10001;
  const MsgAdminNotice  = 10004;
  const MsgAdminChgServerInfo  = 10007;
  const MsgAdminManualOrderCompletion  = 10008;
  const MsgAdminKickOut  = 10009;
}
class PandaMsgHeader extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
    $this->fields["3"] = "PBString";
    $this->values["3"] = "";
  }
  function MsgID()
  {
    return $this->_get_value("1");
  }
  function set_MsgID($value)
  {
    return $this->_set_value("1", $value);
  }
  function KeyLen()
  {
    return $this->_get_value("2");
  }
  function set_KeyLen($value)
  {
    return $this->_set_value("2", $value);
  }
  function SessionKey()
  {
    return $this->_get_value("3");
  }
  function set_SessionKey($value)
  {
    return $this->_set_value("3", $value);
  }
}
class PandaAdminReq extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBString";
    $this->values["2"] = "";
  }
  function Uid()
  {
    return $this->_get_value("1");
  }
  function set_Uid($value)
  {
    return $this->_set_value("1", $value);
  }
  function AdminString()
  {
    return $this->_get_value("2");
  }
  function set_AdminString($value)
  {
    return $this->_set_value("2", $value);
  }
}
class PandaAdminResp extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBString";
    $this->values["2"] = "";
  }
  function MsgResult()
  {
    return $this->_get_value("1");
  }
  function set_MsgResult($value)
  {
    return $this->_set_value("1", $value);
  }
  function ResultString()
  {
    return $this->_get_value("2");
  }
  function set_ResultString($value)
  {
    return $this->_set_value("2", $value);
  }
}
class PandaAdminMailItem extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBString";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
  }
  function itemid()
  {
    return $this->_get_value("1");
  }
  function set_itemid($value)
  {
    return $this->_set_value("1", $value);
  }
  function count()
  {
    return $this->_get_value("2");
  }
  function set_count($value)
  {
    return $this->_set_value("2", $value);
  }
}
class PandaAdminPlayerID extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function playerid()
  {
    return $this->_get_value("1");
  }
  function set_playerid($value)
  {
    return $this->_set_value("1", $value);
  }
}
class PandaAdminMailReq extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PandaAdminPlayerID";
    $this->values["1"] = array();
    $this->fields["2"] = "PBString";
    $this->values["2"] = "";
    $this->fields["3"] = "PBString";
    $this->values["3"] = "";
    $this->fields["4"] = "PBString";
    $this->values["4"] = "";
    $this->fields["5"] = "PBInt";
    $this->values["5"] = "";
    $this->fields["6"] = "PBInt";
    $this->values["6"] = "";
    $this->fields["7"] = "PBInt";
    $this->values["7"] = "";
    $this->fields["8"] = "PBInt";
    $this->values["8"] = "";
    $this->fields["9"] = "PandaAdminMailItem";
    $this->values["9"] = array();
  }
  function playerlist($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_playerlist()
  {
    return $this->_add_arr_value("1");
  }
  function set_playerlist($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function remove_last_playerlist()
  {
    $this->_remove_last_arr_value("1");
  }
  function playerlist_size()
  {
    return $this->_get_arr_size("1");
  }
  function Title()
  {
    return $this->_get_value("2");
  }
  function set_Title($value)
  {
    return $this->_set_value("2", $value);
  }
  function Content()
  {
    return $this->_get_value("3");
  }
  function set_Content($value)
  {
    return $this->_set_value("3", $value);
  }
  function Sender()
  {
    return $this->_get_value("4");
  }
  function set_Sender($value)
  {
    return $this->_set_value("4", $value);
  }
  function give_diamond()
  {
    return $this->_get_value("5");
  }
  function set_give_diamond($value)
  {
    return $this->_set_value("5", $value);
  }
  function give_gold()
  {
    return $this->_get_value("6");
  }
  function set_give_gold($value)
  {
    return $this->_set_value("6", $value);
  }
  function mail_type()
  {
    return $this->_get_value("7");
  }
  function set_mail_type($value)
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
  function itemlist($offset)
  {
    return $this->_get_arr_value("9", $offset);
  }
  function add_itemlist()
  {
    return $this->_add_arr_value("9");
  }
  function set_itemlist($index, $value)
  {
    $this->_set_arr_value("9", $index, $value);
  }
  function remove_last_itemlist()
  {
    $this->_remove_last_arr_value("9");
  }
  function itemlist_size()
  {
    return $this->_get_arr_size("9");
  }
}
class PandaAdminMailResp extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function MsgResult()
  {
    return $this->_get_value("1");
  }
  function set_MsgResult($value)
  {
    return $this->_set_value("1", $value);
  }
}
class PandaGMChangeAccountStateReq extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
    $this->fields["3"] = "PBString";
    $this->values["3"] = "";
  }
  function Uid()
  {
    return $this->_get_value("1");
  }
  function set_Uid($value)
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
  function rolename()
  {
    return $this->_get_value("3");
  }
  function set_rolename($value)
  {
    return $this->_set_value("3", $value);
  }
}
class PandaGMChangeAccountStateResp extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
    $this->fields["3"] = "PBString";
    $this->values["3"] = "";
    $this->fields["4"] = "PBString";
    $this->values["4"] = "";
    $this->fields["5"] = "PBInt";
    $this->values["5"] = "";
    $this->fields["6"] = "PBInt";
    $this->values["6"] = "";
    $this->fields["7"] = "PBInt";
    $this->values["7"] = "";
    $this->fields["8"] = "PBString";
    $this->values["8"] = "";
    $this->fields["9"] = "PBInt";
    $this->values["9"] = "";
    $this->fields["10"] = "PBInt";
    $this->values["10"] = "";
  }
  function MsgResult()
  {
    return $this->_get_value("1");
  }
  function set_MsgResult($value)
  {
    return $this->_set_value("1", $value);
  }
  function Uid()
  {
    return $this->_get_value("2");
  }
  function set_Uid($value)
  {
    return $this->_set_value("2", $value);
  }
  function Account()
  {
    return $this->_get_value("3");
  }
  function set_Account($value)
  {
    return $this->_set_value("3", $value);
  }
  function RoleName()
  {
    return $this->_get_value("4");
  }
  function set_RoleName($value)
  {
    return $this->_set_value("4", $value);
  }
  function Flag()
  {
    return $this->_get_value("5");
  }
  function set_Flag($value)
  {
    return $this->_set_value("5", $value);
  }
  function LastLoginTime()
  {
    return $this->_get_value("6");
  }
  function set_LastLoginTime($value)
  {
    return $this->_set_value("6", $value);
  }
  function LastLogoutTime()
  {
    return $this->_get_value("7");
  }
  function set_LastLogoutTime($value)
  {
    return $this->_set_value("7", $value);
  }
  function IP()
  {
    return $this->_get_value("8");
  }
  function set_IP($value)
  {
    return $this->_set_value("8", $value);
  }
  function BanTime()
  {
    return $this->_get_value("9");
  }
  function set_BanTime($value)
  {
    return $this->_set_value("9", $value);
  }
  function UnBanTime()
  {
    return $this->_get_value("10");
  }
  function set_UnBanTime($value)
  {
    return $this->_set_value("10", $value);
  }
}
class PandaGMNoticeReq extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBString";
    $this->values["1"] = "";
  }
  function content()
  {
    return $this->_get_value("1");
  }
  function set_content($value)
  {
    return $this->_set_value("1", $value);
  }
}
class PandaGMNoticeResp extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function MsgResult()
  {
    return $this->_get_value("1");
  }
  function set_MsgResult($value)
  {
    return $this->_set_value("1", $value);
  }
}
class PandaChangeServerInfoReq extends PBMessage
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
  function StartTime()
  {
    return $this->_get_value("1");
  }
  function set_StartTime($value)
  {
    return $this->_set_value("1", $value);
  }
  function ServerID()
  {
    return $this->_get_value("2");
  }
  function set_ServerID($value)
  {
    return $this->_set_value("2", $value);
  }
}
class PandaChangeServerInfoResp extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function MsgResult()
  {
    return $this->_get_value("1");
  }
  function set_MsgResult($value)
  {
    return $this->_set_value("1", $value);
  }
}
class PandaAdminManualOrderCompletionReq extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBString";
    $this->values["2"] = "";
    $this->fields["3"] = "PBInt";
    $this->values["3"] = "";
  }
  function Uid()
  {
    return $this->_get_value("1");
  }
  function set_Uid($value)
  {
    return $this->_set_value("1", $value);
  }
  function UniOrderID()
  {
    return $this->_get_value("2");
  }
  function set_UniOrderID($value)
  {
    return $this->_set_value("2", $value);
  }
  function ProductPrice()
  {
    return $this->_get_value("3");
  }
  function set_ProductPrice($value)
  {
    return $this->_set_value("3", $value);
  }
}
class PandaAdminManualOrderCompletionResp extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function MsgResult()
  {
    return $this->_get_value("1");
  }
  function set_MsgResult($value)
  {
    return $this->_set_value("1", $value);
  }
}
class PandaAdminKickOutReq extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function Uid()
  {
    return $this->_get_value("1");
  }
  function set_Uid($value)
  {
    return $this->_set_value("1", $value);
  }
}
class PandaAdminKickOutResp extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function MsgResult()
  {
    return $this->_get_value("1");
  }
  function set_MsgResult($value)
  {
    return $this->_set_value("1", $value);
  }
}
?>