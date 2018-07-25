 
//document.write("<script language=javascript src='../dep/lib/layui/layui.all.js'></script>");

/*
*@ demo数据
*/
function init_online_check_login_status()
{
  var storage = window.localStorage;
  //console.log(storage);
  if(storage.account == null){
        window.location.href="../login.html"; 
    }
}

/*
*@ 刷新页面
*/
function renderForm(){
  layui.use('form', function(){
   var form = layui.form;//高版本建议把括号去掉，有的低版本，需要加()  当前为高版本
   form.render();
  });
}


/*
*@ demo数据
*/
// function save_data_to_session(json)
// {
//     var storage = window.localStorage;
//     var ser_list  = new Array();
//     for (var i = 0; i < json.length; i++)
//     {
//         ser_list.push({"name":json[i].server_name,"id":json[i].server_id})
//     }
    
//     //console.log(ser_list);
//     storage.setItem("ser_list", JSON.stringify(ser_list));
//     //console.log(storage);
// }

function save_data_to_session(json,type)
{
    var storage = window.localStorage;
    var list  = new Array();
    if (type == 'server_list') {
        for (var i = 0; i < json.length; i++)
        {
            list.push({"name":json[i].server_name,"id":json[i].server_id})
        }
        // console.log(list);
        storage.setItem("ser_list", JSON.stringify(list));
    } else {
        for (var i = 0; i < json.length; i++)
        {
            list.push({"name":json[i].channel_name,"id":json[i].mark_type})
        }
        storage.setItem("mark_list", JSON.stringify(list));
    }
}


/*
*@ demo数据
*/
function get_server_list_data(type)
{
    var send_url = '../php/php/get_server_list.php';
    if (type == "index") {
        send_url = 'php/php/get_server_list.php';
    } else {
        send_url = '../php/php/get_server_list.php';
    }
    console.log(send_url);
     $.ajax({
          type: "POST",
         url: send_url,
          dataType: "json",
          success: function (json) {
              save_data_to_session(json,'server_list');
          },
          error: function (er) {
              console.log(er);
          },
        });
}

function get_mark_list_data(type)
{
    var send_url = "../php/php/mark.php";
    if (type == "index") {
        send_url = 'php/php/mark.php';
    } else {
        send_url = '../php/php/mark.php';
    }
    console.log(send_url);
    var query_type = 'mark';
     $.ajax({
          type: "POST",
         url: send_url,
          dataType: "json",
          data: {
           "query_type": query_type,
          },
          success: function (json) {
            // console.log(json);
              save_data_to_session(json,'mark_list');
          },
          error: function (er) {
              console.log(er);
          },
        });
}

/*
*@ 动态加载select
*/
function reload_select_list(type)
{
    get_server_list_data(type);
    var storage = window.localStorage;
    var server_id_list = storage.getItem("ser_list");
    var list = JSON.parse(server_id_list);
    return list;
   
}

function get_need_info_and_callback_it(data_base, request_data, call_back)
{
	$.ajax({
		type: "POST",
		url:"../php/php/get_db_data.php",
		dataType:"json",
		data:{
			'database' : data_base,
			'data' : request_data
		},
		success:function(json){
			call_back(json);
		}
	});
}

function get_data_and_callback_it(oper_id, call_back)
{
  $.ajax({
    type: "POST",
    url:"../php/php/get_db_data.php",
    dataType:"json",
    data:{
      'oper_id' : oper_id
    },
    success:function(json){
      call_back(json);
    }
  });
}

function send_msg_to_server(msg_id, msg_data, call_back)
{
  $.ajax({
    type: "POST",
    url:"../php/interface/msg_handler.php",
    dataType:"json",
    data:{
      'msg_id' : msg_id,
      'msg_data' : JSON.stringify(msg_data)
    },
    success:function(res){
      if(call_back !== undefined){
        call_back(res);
      }
    },
    error:function(err){
      console.log('1111111111122222');
      console.log('ERROR:' + msg_id + msg_data);
      console.log(err);
    }
  });
}

function get_server_list_with_callback(callback)
{
  $.ajax({
          type: "POST",
          url: '../php/php/get_server_list.php',
          dataType: "json",
          success: function (json) {
            console.log("callback");
            console.log(json.length);
            var list  = new Array();
            for (var i = 0; i < json.length; i++)
            {
                list.push({"name":json[i].server_name,"id":json[i].server_id})
            }
            callback(list);
          },
          error: function (er) {
              console.log(er);
          },
        });
}

function reload_mark_list(type)
{
    get_mark_list_data(type);
    var storage = window.localStorage;
    var mark_id_list = storage.getItem("mark_list");
    var list = JSON.parse(mark_id_list);
    return list;
   
}


function show_list(data)
{
  var gm_tools_div = document.getElementById("gm_tools");
  var operation_div = document.getElementById("operation");
  if (data == "admin") {
    gm_tools_div.setAttribute("style","display");
    operation_div.setAttribute("style","display");
  } else if(data == "test"){
    operation_div.setAttribute("style","display");
  }
}

function cache_change_mark(mark) {
    var storage = window.localStorage;
    storage["mark"] = mark;
}

function get_cache_mark() {
    var storage = window.localStorage;
    return storage.mark;
}
