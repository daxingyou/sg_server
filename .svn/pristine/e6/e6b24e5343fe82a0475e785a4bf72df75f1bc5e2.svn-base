/*
 *@ 数值不足10 补0
*/
function addzero(v) {
	if (v < 10) return '0' + v;
	return v.toString();
}


function get_date(param_time) {
	if (undefined == param_time) {
		var time = new Date();
		var date = time.getFullYear() + '-' + addzero(time.getMonth() + 1) + '-' + addzero(time.getDate());
		console.log(date);
		return date;
	} else {
		var date = param_time.getFullYear() + '-' + addzero(param_time.getMonth() + 1) + '-' + addzero(param_time.getDate());
		console.log(date);
		return date;
	}
}



function get_time_to_date(days_1, days_2) {
	//console.log(days_1,days_2);
	var time = new Date().getTime();
	var time_max = time - get_msec_time(days_1);
	var time_min = time - get_msec_time(days_2);
	var cur_max = new Date(time_max);
	var cur_min = new Date(time_min);
	//console.log(cur_max,cur_min);
	document.getElementById('logmax').value = get_date(cur_max);
	document.getElementById('logmin').value = get_date(cur_min);
}


/*
 *@ 获取毫秒
*/
function get_msec_time(value) {
	// console.log(value);
	// var is_number = regex_number(parseInt(value));
	// if (is_number == null) {
	// 	return 0;
	// } else {
	// 	return (value * 24 * 3600 * 1000);	
	// }
	return (value * 24 * 3600 * 1000);
}

/*
 *@ 获取本地时间
*/
function getLocalTime(nS) {
	return new Date(parseInt(nS) * 1000).toLocaleString().replace(/年|月/g, "-").replace(/日/g, " ");
}

/*
 *@ regex
*/
function regex_number(value) {
	var is_number = value.match(/^[1-9]+[0-9]*]*$/);
	return is_number;
}

/*
 *@ 判断是否是DID
*/
function regex_is_DID(value) {
	var is_did = value.match(/^[0-9]{7}$/);
	return is_did;
}

/*
 *@ 判断是否是DID
*/
function regex_value(value) {
	var is_did = value.match(/^[1-9]{7}$/);
	return is_did;
}


/*
 *@ Intput show result
*/
function Intput_Show_Result(type) {
	switch (type) {
		case 'ban':
			{
				var show_end_select_time = $('#show_end_select_time').val();
				var opt_time = $('#opt_time').val();
				if (opt_time != '') {
					console.log(opt_time);
					var is_number = regex_number(opt_time);
					console.log(is_number);
					if (is_number == null) {
						document.getElementById('show_end_select_time').innerText = '';
						renderForm();
						return;
					}
					var show_time = new Date(new Date().getTime() + (opt_time * 60 * 1000));
					var show = show_time.getFullYear() + '-' + addzero(show_time.getMonth() + 1) + '-' + addzero(show_time.getDate()) + ' ' + addzero(show_time.getHours()) + ':' + addzero(show_time.getMinutes()) + ':' + addzero(show_time.getSeconds());
					console.log(show);
					document.getElementById('show_end_select_time').innerText = "时间截止到:" + show;
					renderForm();
				} else {
					document.getElementById('show_end_select_time').innerText = '';
					renderForm();
				}
			}
			break;
		default:
			// statements_def
			break;
	}

}


/*
 *@ 数值不足10 补0
*/
function Did_Or_Name_Input_Change(type) {
	if (0 == type) {
		console.log("+++++++++++++++++++++++++");
		console.log(type);
		var role_did = $('#role_did').val();
		var role_name = $('#role_name').val();

		if (role_name != "") {
			$('#role_did').attr('disabled', true);
		} else if (role_did != "") {
			$('#role_name').attr('disabled', true);
		}

		if (role_did == "" && role_name == "") {
			$('#role_did').removeAttr('disabled');
			$('#role_name').removeAttr('disabled');
		}
	} else {
		console.log("+++++++++++++++++++++++++");
		console.log(type);
		var role_did_1 = $('#role_did_1').val();
		var role_name_1 = $('#role_name_1').val();

		if (role_name_1 != "") {
			$('#role_did_1').attr('disabled', true);
		} else if (role_did_1 != "") {
			$('#role_name_1').attr('disabled', true);
		}

		if (role_did_1 == "" && role_name_1 == "") {
			$('#role_did_1').removeAttr('disabled');
			$('#role_name_1').removeAttr('disabled');
		}
	}
}



/*
 *@ 表格数据修饰
*/
function data_mod(json, type) {
	switch (type) {
		case 'ban':
			{
				for (var i = 0; i < json.length; i++) {
					if (json[i].manage_type == "0") {
						json[i].manage_type = "账号封禁";
					} else if (json[i].manage_type == "1") {
						json[i].manage_type = "解封账号";
					} else if (json[i].manage_type == "2") {
						json[i].manage_type = "角色禁言";
					} else if (json[i].manage_type == "3") {
						json[i].manage_type = "解除禁言";
					}
					if (json[i].offset_time == 0) {
						if (json[i].manage_type == "解封账号" || json[i].manage_type == "解除禁言") {
							json[i].offset_time = '';
						} else if (json[i].manage_type == "账号封禁" || json[i].manage_type == "角色禁言") {
							json[i].offset_time = '永久';
						}
					}

					if (json[i].expired_time != 0) {
						var expired_time = new Date(parseInt(json[i].expired_time) * 1000);
						json[i].expired_time = (expired_time.getFullYear() + '-' + addzero(expired_time.getMonth() + 1) + '-' + addzero(expired_time.getDate()) + ' ' + addzero(expired_time.getHours()) + ':' + addzero(expired_time.getMinutes()) + ':' + addzero(expired_time.getSeconds()));
					} else if (json[i].expired_time == 0) {
						json[i].expired_time = '';
					}

					var oper_time = new Date(parseInt(json[i].oper_time) * 1000);
					json[i].oper_time = (oper_time.getFullYear() + '-' + addzero(oper_time.getMonth() + 1) + '-' + addzero(oper_time.getDate()) + ' ' + addzero(oper_time.getHours()) + ':' + addzero(oper_time.getMinutes()) + ':' + addzero(oper_time.getSeconds()));
				}
				return json;
			}
			break;
		case 'base_info':
			{

				var server_id_list = $('#zcySelect').val();
				json.server_id = server_id_list;
				var Json = new Array(json);
				console.log(Json);
				for (var i = 0; i < Json.length; i++) {
					if (Json[0].create_time != 0) {
						var create_time = new Date(parseInt(Json[0].create_time) * 1000);
						Json[0].create_time = (create_time.getFullYear() + '-' + addzero(create_time.getMonth() + 1) + '-' + addzero(create_time.getDate()) + ' ' + addzero(create_time.getHours()) + ':' + addzero(create_time.getMinutes()) + ':' + addzero(create_time.getSeconds()));
					} else if (Json[0].create_time == 0) {
						Json[0].create_time = '';
					}

					var last_logout_time = new Date(parseInt(Json[0].last_logout_time) * 1000);
					Json[0].last_logout_time = (last_logout_time.getFullYear() + '-' + addzero(last_logout_time.getMonth() + 1) + '-' + addzero(last_logout_time.getDate()) + ' ' + addzero(last_logout_time.getHours()) + ':' + addzero(last_logout_time.getMinutes()) + ':' + addzero(last_logout_time.getSeconds()));
				}
				return Json;
			}
			break;
		case 'recharge_log':
			{
				for (var i = 0; i < json.length; i++) {
					if (json[i].create_time != 0) {
						var create_time = new Date(parseInt(json[i].create_time) * 1000);
						json[i].create_time = (create_time.getFullYear() + '-' + addzero(create_time.getMonth() + 1) + '-' + addzero(create_time.getDate()) + ' ' + addzero(create_time.getHours()) + ':' + addzero(create_time.getMinutes()) + ':' + addzero(create_time.getSeconds()));
					} else if (json[i].create_time == 0) {
						json[i].create_time = '';
					}

				}
				return json;
			}
			break;
		case 'mail_log':
			{
				for (var i = 0; i < json.length; i++) {
					if (json[i].send_time != 0) {
						var send_time = new Date(parseInt(json[i].send_time) * 1000);
						json[i].send_time = (send_time.getFullYear() + '-' + addzero(send_time.getMonth() + 1) + '-' + addzero(send_time.getDate()) + ' ' + addzero(send_time.getHours()) + ':' + addzero(send_time.getMinutes()) + ':' + addzero(send_time.getSeconds()));
					} else if (json[i].send_time == 0) {
						json[i].send_time = '';
					}

					if (json[i].oper_time != 0) {
						var oper_time = new Date(parseInt(json[i].oper_time) * 1000);
						json[i].oper_time = (oper_time.getFullYear() + '-' + addzero(oper_time.getMonth() + 1) + '-' + addzero(oper_time.getDate()) + ' ' + addzero(oper_time.getHours()) + ':' + addzero(oper_time.getMinutes()) + ':' + addzero(oper_time.getSeconds()));
					} else if (json[i].oper_time == 0) {
						json[i].oper_time = '';
					}

					if (json[i].on_time != 0) {
						var on_time = new Date(parseInt(json[i].on_time) * 1000);
						json[i].on_time = (on_time.getFullYear() + '-' + addzero(on_time.getMonth() + 1) + '-' + addzero(on_time.getDate()) + ' ' + addzero(on_time.getHours()) + ':' + addzero(on_time.getMinutes()) + ':' + addzero(on_time.getSeconds()));
					} else if (json[i].on_time == 0) {
						json[i].on_time = '';
					}

					if (json[i].end_time != 0) {
						var end_time = new Date(parseInt(json[i].end_time) * 1000);
						json[i].end_time = (end_time.getFullYear() + '-' + addzero(end_time.getMonth() + 1) + '-' + addzero(end_time.getDate()) + ' ' + addzero(end_time.getHours()) + ':' + addzero(end_time.getMinutes()) + ':' + addzero(end_time.getSeconds()));
					} else if (json[i].end_time == 0) {
						json[i].end_time = '';
					}

					if (json[i].id == 0) {
						json[i].id = '全服';
					}

				}
				return json;
			}
			break;
		case 'notice_log':
			{
				for (var i = 0; i < json.length; i++) {
					if (json[i].start_time != 0) {
						var start_time = new Date(parseInt(json[i].start_time) * 1000);
						json[i].start_time = (start_time.getFullYear() + '-' + addzero(start_time.getMonth() + 1) + '-' + addzero(start_time.getDate()) + ' ' + addzero(start_time.getHours()) + ':' + addzero(start_time.getMinutes()) + ':' + addzero(start_time.getSeconds()));
					} else if (json[i].start_time == 0) {
						json[i].start_time = '';
					}

					if (json[i].end_time != 0) {
						var end_time = new Date(parseInt(json[i].end_time) * 1000);
						json[i].end_time = (end_time.getFullYear() + '-' + addzero(end_time.getMonth() + 1) + '-' + addzero(end_time.getDate()) + ' ' + addzero(end_time.getHours()) + ':' + addzero(end_time.getMinutes()) + ':' + addzero(end_time.getSeconds()));
					} else if (json[i].end_time == 0) {
						json[i].end_time = '';
					}

					if (json[i].oper_time != 0) {
						var oper_time = new Date(parseInt(json[i].oper_time) * 1000);
						json[i].oper_time = (oper_time.getFullYear() + '-' + addzero(oper_time.getMonth() + 1) + '-' + addzero(oper_time.getDate()) + ' ' + addzero(oper_time.getHours()) + ':' + addzero(oper_time.getMinutes()) + ':' + addzero(oper_time.getSeconds()));
					} else if (json[i].oper_time == 0) {
						json[i].oper_time = '';
					}

					if (json[i].tick == 0) {
						json[i].tick = '';
					}

				}
				return json;
			}
			break;
		case 'mark_login':
			{
				for (var i = 0; i < json.length; i++) {
					//console.log(json[i]);
					if (json[i].label == 0) {
						json[i].label = '普通';
					} else if (json[i].label == 1) {
						json[i].label = '新服';
					} else if (json[i].label == 2) {
						json[i].label = '推荐';
					}

					if (json[i].status == 0) {
						json[i].status = '删除';
					} else if (json[i].status == 1) {
						json[i].status = '空闲';
					} else if (json[i].status == 2) {
						json[i].status = '忙碌';
					} else if (json[i].status == 3) {
						json[i].status = '火爆';
					} else if (json[i].status == 4) {
						json[i].status = '维护';
					}

				}
				return json;
			}
			break;
		default:
			break;
	}
}


/*
 *@ 获取秒数
*/

function get_msec_to_sec(sec_time) {
	var msec = 0;
	console.log(sec_time);
	if (0 == sec_time || undefined == sec_time) {
		msec = new Date().getTime();
	} else {
		msec = new Date(sec_time).getTime();
	}

	var sec = parseInt(msec / 1000);
	// console.log(msec,sec);
	return sec;
}

function get_db_info(id) {
	console.log(id);
	var send = new Object();
	switch (id) {
		case "1":
			{
				send.log_host = "192.168.1.5";
				send.log_pwd = "Sanguo1!";
				send.log_user = "root";
				send.log_db = "nei_trunk_sg_log";
			}
			break;
		case "2":
			{
				send.log_host = "192.168.1.6";
				send.log_pwd = "Sanguo1!";
				send.log_user = "root";
				send.log_db = "design_trunk_sg_log";
			}
			break;
		case "3":
			{
				send.log_host = "139.196.41.108";
				send.log_pwd = "Fengtu2018!";
				send.log_user = "fengtu";
				send.log_db = "wai_trunk_sg_log";
			}
			break;
		default:
			break;
	}
	console.log(send);
	return send;
}

function Game_Oper_Data_Db_Info(id) {
	console.log(id);
	var send = new Object();
	switch (id) {
		case "1":
			{
				send.log_host = "192.168.1.5";
				send.log_pwd = "Sanguo1!";
				send.log_user = "root";
				send.log_db = "nei_trunk_sg_log";
			}
			break;
		case "2":
			{
				send.log_host = "192.168.1.6";
				send.log_pwd = "Sanguo1!";
				send.log_user = "root";
				send.log_db = "design_trunk_sg_log";
			}
			break;
		case "3":
			{
				send.log_host = "139.196.41.108";
				send.log_pwd = "Fengtu2018!";
				send.log_user = "fengtu";
				send.log_db = "wai_trunk_sg_log";
			}
			break;
		default:
			break;
	}
	console.log(send);
	return send;
}


function get_game_daily_show_map_data(json, type) {
	var list = new Array();
	switch (type) {
		case 'new_role':
			{
				var mobile = new Object();
				mobile.name = "新增设备数量(个)";
				mobile.data = [];

				var account = new Object();
				account.name = "新增账号数量(个)";
				account.data = [];

				var role = new Object();
				role.name = "新增角色数量(个)";
				role.data = [];

				// var rate = new Object();
				// rate.name = "设备账号转化率(%)";
				// rate.data = [];

				// var reg_mobile = new Object();
				// reg_mobile.name = "注册设备数量";
				// reg_mobile.data = [];

				list.push(mobile);
				list.push(account);
				list.push(role);
				// list.push(rate);
				// list.push(reg_mobile);
				for (var i = 0; i < json.length; i++) {
					list[0].data.push(parseInt(json[i].mobile));
					list[1].data.push(parseFloat(json[i].account));
					list[2].data.push(parseInt(json[i].role));
					// list[3].data.push(parseInt(json[i].rate));
					// list[4].data.push(parseInt(json[i].reg_mobile));
				}
			}
			break;

		case 'active_role':
			{
				var dau = new Object();
				dau.name = "日活跃数量(个)";
				dau.data = [];

				var wau = new Object();
				wau.name = "周活跃数量(个)";
				wau.data = [];

				var mau = new Object();
				mau.name = "月活跃数量(个)";
				mau.data = [];

				var dm = new Object();
				dm.name = "日活跃占月活跃的百分比(%)";
				dm.data = [];

				list.push(dau);
				list.push(wau);
				list.push(mau);
				list.push(dm);

				for (var i = 0; i < json.length; i++) {
					list[0].data.push(parseInt(json[i].dau));
					list[1].data.push(parseFloat(json[i].wau));
					list[2].data.push(parseInt(json[i].mau));
					list[3].data.push(parseInt(json[i].dm));
				}
			}
			break;

		case 'retained_role':
			{
				var second_day = new Object();
				second_day.name = "次日留存(%)";
				second_day.data = [];

				var seventh_day = new Object();
				seventh_day.name = "七日留存(%)";
				seventh_day.data = [];

				var thirtieth_day = new Object();
				thirtieth_day.name = "三十日留存(%)";
				thirtieth_day.data = [];


				list.push(second_day);
				list.push(seventh_day);
				list.push(thirtieth_day);
				for (var i = 0; i < json.length; i++) {
					list[0].data.push(parseInt(json[i].second_day));
					list[1].data.push(parseFloat(json[i].seventh_day));
					list[2].data.push(parseInt(json[i].thirtieth_day));
				}
			}
			break;

		case 'loss_role':
			{
				var leave_count = new Object();
				leave_count.name = "流失数(个)";
				leave_count.data = [];

				var leave_rate = new Object();
				leave_rate.name = "流失率(%)";
				leave_rate.data = [];

				var return_count = new Object();
				return_count.name = "回流数(个)";
				return_count.data = [];

				list.push(leave_count);
				list.push(leave_rate);
				list.push(return_count);
				for (var i = 0; i < json.length; i++) {
					list[0].data.push(parseInt(json[i].leave_count));
					list[1].data.push(parseFloat(json[i].leave_rate));
					list[2].data.push(parseInt(json[i].return_count));
				}
			}
			break;
		default:
			break;
	}
	return list;
}

function show_map(json, type) {
	var date_list = new Array();
	for (var i = 0; i < json.length; i++) {
		date_list.push(json[i].time);
	}
	switch (type) {
		case 'new_role':
			{
				Highcharts.chart('map_show_new', {
					title: {
						text: '新增玩家相关图形统计图',
						x: -20 //center
					},
					xAxis: {
						categories: date_list
					},
					yAxis: {
						title: {
							text: '数据统计'
						},
						plotLines: [{
							value: 0,
							width: 1,
							color: '#808080'
						}]
					},
					tooltip: {
						valueSuffix: ''
					},
					legend: {
						layout: 'vertical',
						align: 'right',
						verticalAlign: 'middle',
						borderWidth: 0
					},
					series: get_game_daily_show_map_data(json, "new_role")
				});
			}
			break;
		case 'active_role':
			{
				Highcharts.chart('map_show_active', {
					title: {
						text: '活跃玩家相关图形统计图',
						x: -20 //center
					},
					xAxis: {
						categories: date_list
					},
					yAxis: {
						title: {
							text: '数据统计'
						},
						plotLines: [{
							value: 0,
							width: 1,
							color: '#808080'
						}]
					},
					tooltip: {
						valueSuffix: ''
					},
					legend: {
						layout: 'vertical',
						align: 'right',
						verticalAlign: 'middle',
						borderWidth: 0
					},
					series: get_game_daily_show_map_data(json, "active_role")
				});
			}
			break;
		case 'retained_role':
			{
				Highcharts.chart('map_show_retained', {
					title: {
						text: '玩家留存相关图形统计图',
						x: -20 //center
					},
					xAxis: {
						categories: date_list
					},
					yAxis: {
						title: {
							text: '数据统计'
						},
						plotLines: [{
							value: 0,
							width: 1,
							color: '#808080'
						}]
					},
					tooltip: {
						valueSuffix: ''
					},
					legend: {
						layout: 'vertical',
						align: 'right',
						verticalAlign: 'middle',
						borderWidth: 0
					},
					series: get_game_daily_show_map_data(json, "retained_role")
				});
			}
			break;
		case 'loss_role':
			{
				Highcharts.chart('map_show_loss', {
					title: {
						text: '玩家流失相关图形统计图',
						x: -20 //center
					},
					xAxis: {
						categories: date_list
					},
					yAxis: {
						title: {
							text: '数据统计'
						},
						plotLines: [{
							value: 0,
							width: 1,
							color: '#808080'
						}]
					},
					tooltip: {
						valueSuffix: ''
					},
					legend: {
						layout: 'vertical',
						align: 'right',
						verticalAlign: 'middle',
						borderWidth: 0
					},
					series: get_game_daily_show_map_data(json, "loss_role")
				});
			}
			break;
		default:
			break;
	}

}


function get_textarea_limit_value(type) {
	switch (type) {
		case 'mail':
			{
				return 200;
			}
			break;
		case 'notice':
			{
				return 200;
			}
		case 'login_notice':
			{
				return 1500;
			}
		default:
			{
				return 200;
			}
			break;
	}
}


function is_ip(value) {
	var reg = /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/;
	var result = reg.test(value);
	return result;
}