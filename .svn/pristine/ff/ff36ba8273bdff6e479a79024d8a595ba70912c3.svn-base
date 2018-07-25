<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>配置热更新</title>
  <link rel="stylesheet" href="../dep/lib/layui/css/layui.css">
</head>
<body>
<table class="layui-table">
	<colgroup>
    <col width="150">
    <col width="200">
    <col>
  </colgroup>
  <thead>
    <tr>
      <th>参数名</th>
      <th>说明</th>
      <th>值</th>
      <th>可否热更新</th>
    </tr> 
  </thead>
  <tbody>

<?php
require_once("../sg_gm/db_connect.php");
require_once("../sg_gm/lib/log_debug.php");
function genHTMLString($input_string, $val, $comment, $can_modify, &$array_save_data, &$array_can_be_updated){
	$html_string = '<tr>';
	$html_string .= '<td>' . $input_string . '</td>';
	$html_string .= '<td>' . $comment . '</td>';
	$html_string .= '<td ondblclick=\'change_value("' . $input_string . '","' . $val . '")\' id=' . $input_string . '>' . $val . '</td>';
	$yes_or_no = '否';
	if($can_modify == 1){
		$yes_or_no = '是';
	}
	$html_string .= '<td>' . $yes_or_no . '</td>';
	$html_string .= '</tr>';
	// $html_string .= '<div class="layui-inline">';
	// $html_string .= 	'<label class="layui-form-label" >' . $comment . '<br>' . $input_string . ':<br>' . $val . '</label><br>';


	// if($can_modify == 1){
	// 	$html_string = 	'<div class="layui-form-item">';
	// 	$html_string .=  	'<div class="layui-input-inline">';
	// 	$html_string .=  		'<input type="text" name="' . $input_string . '" autocomplete="off" class="layui-input">';
	// 	$html_string .= 	'</div>';
	// 	$html_string .= '</div>';
	// 	// $html_string .=  	'<button class="layui-btn" lay-submit lay-filter="' . $input_string . '-modify">修改</button>';
	// 	// $html_string .=  	'<button class="layui-btn" lay-submit lay-filter="' . $input_string . '-revert">还原</button>';
	// }
	
	//$html_string .= 	'</div>';
	//echo $html_string;
	$array_save_data[$input_string] = $html_string;
	if($can_modify == 1){
		array_push($array_can_be_updated, $input_string);
	}
}

function moveMysqlToHTML($table_name){
	$con = get_connect("server_config");	
	if(!$con){
		die("Could not connect: " . mysql_error());
	}
	
	$ret = $con->query("SELECT * FROM {$table_name}");
	if($ret === false){
		die("query error:" . $con->error);
	}
	log_debug($table_name);
	// $store_result = $con->store_result();
	// if($store_result === false){
	// 	log_debug("6666666666666666666");
	// 	die("no data");
	// }
	$array_html_string = array();
	$array_can_be_updated = array();
	$i = 0;
	while($array_data = $ret->fetch_array()){
		$i++;
		$name = $array_data['name'];
		$value = $array_data['value'];
		$show = $array_data['show'];
		$hot_update = $array_data['hot_update'];
		$comment = $array_data['comment'];
		// echo $name . "\n";
		// echo $value . "\n";
		// echo $show . "\n";
		// echo gettype($show);
		// echo $hot_update . "\n";
		// echo $comment . "\n";
		if($show == 1){
			if(genHTMLString($name, $value, $comment, $hot_update, $array_html_string, $array_can_be_updated) === false){
				echo "Not Be Inserted into XML!\nname:" . $name . "\nvalue:" . $value . "\n";
			}
		}
		// if($i === 2)
		// 	break;
	}
	$con->close();
	foreach($array_html_string as $key=>$html_string){
		echo $html_string;
	}

	echo '</tbody>';
	echo '</table>';
	// echo '<form class="layui-form" action="">';
	// echo '<div class="layui-form-item">';
	// echo '<label class="layui-form-label">选择参数</label>';
	// echo '<div class="layui-input-block">';
	// echo '  <select name="label_name" lay-verify="required" lay-search lay-filter="choose">';
	// echo '    <option value=""></option>';
	// for($i = 0; $i < count($array_can_be_updated); $i++){
	// 	echo '<option value="' . $array_can_be_updated[$i] . '">' . $array_can_be_updated[$i] . '</option>'; 
	// }
	// echo '  </select>';
	// echo '</div>';
	// echo '</div>';
	
}

$server_id = $_POST["server_id"];
$table_name = "table" . $server_id;

moveMysqlToHTML($table_name);
	echo '<script>';
	echo 'var server_id = "' . $server_id . '";';
	echo 'var table_name = "' . $table_name . '"';
	echo '</script>';
?>

<!-- <div class="layui-form-item">
    <label class="layui-form-label">输入值</label>
    <div class="layui-input-block">
      <input type="text" name="value" required  lay-verify="required" placeholder="" autocomplete="off" class="layui-input">
    </div>
</div> -->

<div class="layui-form-item">
	<div class="layui-input-block">
	  <button class="layui-btn" lay-submit lay-filter="formDemo">更改</button>
	  <!-- <button type="reset" class="layui-btn layui-btn-primary">重置</button> -->
	  <button class="layui-btn layui-btn-danger" onclick="disp_confirm('game_server')">生成xml并重载GameServer</button>
	  <button class="layui-btn layui-btn-danger" onclick="disp_confirm('center_server')">生成xml并重载CenterServer</button>
	</div>
</div>

</form>

<!--_footer 作为公共模版分离出去-->
<script type="text/javascript" src="../dep/lib/jquery/1.9.1/jquery.min.js"></script> 
<script type="text/javascript" src="../dep/static/h-ui/js/H-ui.min.js"></script> 
<script type="text/javascript" src="../dep/static/h-ui.admin/js/H-ui.admin.js"></script> <!--/_footer 作为公共模版分离出去-->

<script type="text/javascript" src="../dep/lib/My97DatePicker/4.8/WdatePicker.js"></script>
<script type="text/javascript" src="../dep/lib/datatables/1.10.0/jquery.dataTables.min.js"></script> 
<script type="text/javascript" src="../dep/lib/layui/layui.js" charset="utf-8"></script>
<script type="text/javascript" src="../dep/js/server_list_common.js" charset="utf-8"></script>
<script type="text/javascript" src="../dep/js/common.js" charset="utf-8"></script>

<script>
var row_changed = {};
function change_value(id, val)
{
	var r = prompt("请输入修改的值", val);
	if(r === null)
		return;
	document.getElementById(id).innerHTML = r;
	//var qid = '#' + id;
	//console.log(qid + ',' + $("#server.center_server.connect.transfer.ip").text());
	row_changed[id] = r;
	renderForm();
}

function disp_confirm(server_type)
{
	var r=confirm("你确定要重载" + server_type + "的表格吗?")
	if (r==true)
	{
		//document.write("You pressed OK!")
		$.ajax({
          type: "POST",
          url: '../php/sg_gm/reload_xml.php',
          dataType: "json",
          data: {"server_name" : table_name,
      			 "server_type" : server_type,
      			 "server_id" : server_id},
          success: function (json) {
              console.log(json)
              //page(data_mod(json,'recharge_log'),1);
              //table.render();
              layer.msg("成功", { icon: 6, time: 2000 });
          },
          error: function (er) {
              console.log(er);
              //renderForm();
              layer.msg("错误", { icon: 5, time: 2000 });
          },
        });
	}
}

//Demo
layui.use('form', function(){
  var form = layui.form;
  
  //监听选择
  // form.on('select(choose)', function(data){
	 //  console.log(data.elem); //得到select原始DOM对象
	 //  console.log(data.value); //得到被选中的值
	 //  console.log(data.othis); //得到美化后的DOM对象
	 //  form.val("formDemo", "abcdef");
  // });      

  //监听提交
  form.on('submit(formDemo)', function(data){
  	if(Object.keys(row_changed).length === 0){
  		layer.msg("错误：没有要更改的数据", { icon: 5, time: 2000 });
  		return false;
  	}
    layer.msg(JSON.stringify(data.field));
    console.log(data.field);
    console.log(data.form);
    console.log(data.elem);
    var send_data = {};
    send_data.table_name = table_name;
    send_data.changed_data = row_changed;
    $.ajax({
          type: "POST",
          url: '../sg_gm/set_server_config_in_mysql.php',
          dataType: "json",
          data: send_data,
          success: function (json) {
              console.log(json);
              //page(data_mod(json,'recharge_log'),1);
              //table.render();
              layer.msg("成功", { icon: 6, time: 2000 });
              row_changed = {};
          },
          error: function (er) {
              console.log(er);
              //renderForm();
              layer.msg("错误", { icon: 5, time: 2000 });
          },
        });

    return false;
  });
});
</script>

</body>