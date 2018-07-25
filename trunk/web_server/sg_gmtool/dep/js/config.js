
/**
 * 配置说明
 * 依次添加全局配置 @例如 var sg_web = "url" 
 * 然后在switch 添加类型 和 返回目标地址的接口
 */
var sg_update = "http://139.196.41.108:8080/sg_update/";


/**
 * 
 * @param 外部调用的统一接口 
 */
function get_json_config(type) {
    switch (type) {
        case 'reload':
            {
                return get_sg_update_reload_conf();
            }
            break;
        case 'web_notify':
            {
                return get_sg_update_web_notify_conf();
            }
            break;
        default:
            break;
    }
}


/**
 * 
 * @param 返回目标地址的接口  如果需要拼URL 则在接口内部拼
 * 在get_json_config 内调用
 */
function get_sg_update_reload_conf() {

    return sg_update + "reload_db.php";
}

function get_sg_update_web_notify_conf() {

    return sg_update + "send_web_notify.php";
}