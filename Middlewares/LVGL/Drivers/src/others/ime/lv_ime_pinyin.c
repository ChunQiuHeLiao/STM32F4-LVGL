/**
 * @file lv_ime_pinyin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_ime_pinyin_private.h"
#include "../../core/lv_obj_class_private.h"
#if LV_USE_IME_PINYIN != 0

#include "../../lvgl.h"
#include "../../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_ime_pinyin_class)
#define cand_len LV_GLOBAL_DEFAULT()->ime_cand_len

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_ime_pinyin_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_ime_pinyin_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_ime_pinyin_style_change_event(lv_event_t * e);
static void lv_ime_pinyin_kb_event(lv_event_t * e);
static void lv_ime_pinyin_cand_panel_event(lv_event_t * e);

static void init_pinyin_dict(lv_obj_t * obj, const lv_pinyin_dict_t * dict);
static void pinyin_input_proc(lv_obj_t * obj);
static void pinyin_page_proc(lv_obj_t * obj, uint16_t btn);
static char * pinyin_search_matching(lv_obj_t * obj, char * py_str, uint16_t * cand_num);
static void pinyin_ime_clear_data(lv_obj_t * obj);

#if LV_IME_PINYIN_USE_K9_MODE
    static void pinyin_k9_init_data(lv_obj_t * obj);
    static void pinyin_k9_get_legal_py(lv_obj_t * obj, char * k9_input, const char * py9_map[]);
    static bool pinyin_k9_is_valid_py(lv_obj_t * obj, char * py_str);
    static void pinyin_k9_fill_cand(lv_obj_t * obj);
    static void pinyin_k9_cand_page_proc(lv_obj_t * obj, uint16_t dir);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t lv_ime_pinyin_class = {
    .constructor_cb = lv_ime_pinyin_constructor,
    .destructor_cb  = lv_ime_pinyin_destructor,
    .width_def      = LV_SIZE_CONTENT,
    .height_def     = LV_SIZE_CONTENT,
    .group_def      = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .instance_size  = sizeof(lv_ime_pinyin_t),
    .base_class     = &lv_obj_class,
    .name = "ime-pinyin",
};

#if LV_IME_PINYIN_USE_K9_MODE
static const char * lv_btnm_def_pinyin_k9_map[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 21] = {\
                                                                                      ",\0", "123\0",  "abc \0", "def\0",  LV_SYMBOL_BACKSPACE"\0", "\n\0",
                                                                                      ".\0", "ghi\0", "jkl\0", "mno\0",  LV_SYMBOL_KEYBOARD"\0", "\n\0",
                                                                                      "?\0", "pqrs\0", "tuv\0", "wxyz\0",  LV_SYMBOL_NEW_LINE"\0", "\n\0",
                                                                                      LV_SYMBOL_LEFT"\0", "\0"
                                                                                     };

static lv_buttonmatrix_ctrl_t default_kb_ctrl_k9_map[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 17] = { 1 };
static char   lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 2][LV_IME_PINYIN_K9_MAX_INPUT] = {0};
#endif

static char   lv_pinyin_cand_str[LV_IME_PINYIN_CAND_TEXT_NUM][4];
static char * lv_btnm_def_pinyin_sel_map[LV_IME_PINYIN_CAND_TEXT_NUM + 3];

#if LV_IME_PINYIN_USE_DEFAULT_DICT
// /**
//  * LVGL拼音输入法字典（基于3600+常用汉字生成）
//  * 格式：{ 无声调拼音, 对应汉字串 }
//  * 存储：static const → 存Flash，节省RAM
//  * 兼容：用v替代ü（如nv=女，lv=驴）
//  */
// typedef struct {
//     const char* pinyin;
//     const char* chars;
// } lv_pinyin_dict_t;

static const lv_pinyin_dict_t lv_ime_pinyin_def_dict[] = {
    { "a", "啊吖腌锕" },
    { "ai", "爱埃挨哎唉哀皑癌蔼矮艾碍隘捱嗳" },
    { "an", "安暗案岸按俺氨鞍庵谙黯揞桉铵" },
    { "ang", "昂肮盎卬" },
    { "ao", "敖熬翱袄奥傲澳懊坳媪岙" },
    { "ba", "八巴吧拔把坝霸罢扒芭笆疤跋靶" },
    { "bai", "白百柏摆拜败佰稗掰" },
    { "ban", "班斑搬板版半办伴瓣般颁扮" },
    { "bang", "邦帮榜膀绑棒磅蚌镑" },
    { "bao", "包胞苞褒宝保堡报抱暴爆薄" },
    { "bei", "杯悲碑北贝辈背被卑惫焙倍蓓呗" },
    { "ben", "奔本笨苯畚" },
    { "beng", "崩泵蹦甭绷迸" },
    { "bi", "逼鼻比笔彼碧蔽毕毙币必辟弊壁璧秘彼婢弼痹匕吡妣秕俾庳愎滗濞睥嬖璧" },
    { "bian", "边变遍便编贬扁辩辨鞭匾卞汴忭苄砭碥窆" },
    { "biao", "标表彪膘镖飙镳裱" },
    { "bie", "别憋鳖瘪" },
    { "bin", "宾滨彬斌濒缤鬓摈殡膑" },
    { "bing", "冰兵并病丙柄秉饼禀摒邴" },
    { "bo", "波播博薄伯泊勃脖博驳帛菠玻钵跛簸礴啵" },
    { "bu", "不布步部捕簿卜哺补堡埠埔菩埔钚" },
    { "ca", "擦嚓" },
    { "cai", "才材财裁采彩菜猜睬踩蔡" },
    { "can", "参残蚕惨惭餐灿粲孱" },
    { "cang", "仓苍沧舱藏" },
    { "cao", "草操曹槽嘈漕糙" },
    { "ce", "策侧测册厕恻" },
    { "cen", "岑涔" },
    { "ceng", "层曾蹭" },
    { "cha", "叉茶查差插察岔刹诧姹杈槎碴" },
    { "chai", "柴拆豺钗" },
    { "chan", "产缠蝉馋潺婵铲阐颤谄谗禅单冁巉" },
    { "chang", "长场尝常昌畅倡唱肠厂敞裳昶怅伥娼菖徜" },
    { "chao", "超朝潮吵巢炒绰钞晁" },
    { "che", "车扯撤彻掣澈" },
    { "chen", "臣尘陈晨沉辰趁衬称橙忱谌宸碜" },
    { "cheng", "成程称城乘盛诚呈承惩澄橙撑秤骋" },
    { "chi", "吃池尺赤持迟翅斥齿耻侈驰弛炽痴疵茨糍笞哧嗤媸敕眵踟敕" },
    { "chong", "充冲重虫崇宠忡憧铳" },
    { "chou", "抽丑臭绸愁仇瞅酬畴踌惆俦帱雠" },
    { "chu", "出初处除触厨楚础畜储搐矗褚绌刍" },
    { "chuai", "揣踹啜膪" },
    { "chuan", "川传船穿串喘舛钏椽" },
    { "chuang", "创床窗闯疮怆" },
    { "chui", "吹垂锤捶炊陲" },
    { "chun", "春纯唇淳醇蠢椿鹑" },
    { "chuo", "戳绰辍龊" },
    { "ci", "词此刺次慈磁瓷辞赐茨疵雌鹚糍" },
    { "cong", "从匆葱聪囱淙琮枞" },
    { "cou", "凑辏" },
    { "cu", "粗促簇醋猝蹴" },
    { "cuan", "窜篡蹿汆" },
    { "cui", "催脆翠粹瘁崔摧璀淬啐悴" },
    { "cun", "村存寸忖皴" },
    { "cuo", "错搓撮措挫磋厝" },
    { "da", "大达答打搭瘩鞑" },
    { "dai", "带代待袋戴逮殆怠贷埭玳黛傣" },
    { "dan", "单但蛋淡担弹丹胆诞耽郸掸旦氮澹" },
    { "dang", "当党挡荡档铛宕" },
    { "dao", "到道倒岛盗导稻蹈祷捣悼叨" },
    { "de", "得德的地" },
    { "deng", "等登灯邓凳瞪澄磴" },
    { "di", "地第帝低滴底迪敌抵递堤笛狄涤翟嫡缔砥骶娣嗲荻" },
    { "dian", "点电店垫典颠殿奠佃滇巅惦淀玷钿癜" },
    { "diao", "调吊钓雕貂叼刁碉屌" },
    { "die", "跌爹碟叠迭谍蝶迭垤耋鲽" },
    { "ding", "丁顶定钉订鼎锭町" },
    { "diu", "丢" },
    { "dong", "东动冬懂洞栋冻董咚侗垌" },
    { "dou", "都斗豆逗抖陡兜窦蚪" },
    { "du", "度读独杜毒堵督渡肚妒犊笃椟" },
    { "duan", "段短断端锻缎椴" },
    { "dui", "对队堆兑怼敦" },
    { "dun", "吨顿盾蹲敦遁囤盹炖砘" },
    { "duo", "多朵夺躲朵舵惰堕跺掇哆踱" },
    { "e", "鹅蛾额恶厄扼遏俄娥峨讹饿噩呃莪萼鳄颚" },
    { "en", "恩摁嗯蒽" },
    { "er", "而儿耳二尔迩洱饵贰" },
    { "fa", "发法罚伐乏阀珐藩" },
    { "fan", "反犯饭繁凡翻番帆返范樊藩钒贩泛" },
    { "fang", "方放房防访芳仿纺坊肪舫" },
    { "fei", "飞非费肥匪废肺吠沸菲啡诽蜚翡痱" },
    { "fen", "分份奋坟粉芬愤纷粪酚氛汾" },
    { "feng", "风丰封峰缝疯冯枫讽奉凤俸烽酆" },
    { "fo", "佛" },
    { "fou", "否缶" },
    { "fu", "服福付负富复附伏浮佛夫府腐辅父肤赴抚斧符幅傅赋俘涪袱芙馥驸脯蝮" },
    { "ga", "伽嘎咖尬" },
    { "gai", "该改盖概钙丐垓赅" },
    { "gan", "干感甘敢肝杆赶赣竿柑橄矸泔酐" },
    { "gang", "刚钢岗港纲缸肛扛罡" },
    { "gao", "高告搞稿膏糕羔皋镐杲缟" },
    { "ge", "个各哥歌戈割阁隔葛革鸽疙咯舸" },
    { "gei", "给" },
    { "gen", "根跟亘艮" },
    { "geng", "更耕庚羹耿梗埂赓" },
    { "gong", "工公功攻供宫躬龚巩拱贡汞蚣" },
    { "gou", "勾狗够购沟构垢苟钩篝岣" },
    { "gu", "古谷股故固孤姑鼓顾辜菇咕沽锢梏胍" },
    { "gua", "瓜挂寡褂刮卦胍" },
    { "guai", "怪乖拐" },
    { "guan", "关管观惯冠官馆罐贯灌盥莞" },
    { "guang", "光广逛胱" },
    { "gui", "归规贵桂鬼轨柜圭癸硅瑰诡癸刽跪鲑" },
    { "gun", "滚棍衮鲧" },
    { "guo", "国过果郭锅裹聒蝈" },
    { "ha", "哈蛤铪" },
    { "hai", "海害还孩亥骇骸嗨" },
    { "han", "汉汗含寒喊旱憾酣憨邯翰涵悍焊罕菡" },
    { "hang", "行航杭夯巷吭" },
    { "hao", "好号豪耗浩郝毫嚎皓镐" },
    { "he", "和喝河合核何贺荷赫盒禾涸阂貉" },
    { "hei", "黑嘿" },
    { "hen", "很狠恨痕" },
    { "heng", "横恒衡亨哼桁" },
    { "hong", "红洪宏鸿轰哄烘虹讧薨" },
    { "hou", "后侯厚候喉猴吼逅" },
    { "hu", "胡户湖护呼互乎忽虎狐壶沪唬扈祜醐" },
    { "hua", "花华化画话滑哗桦猾" },
    { "huai", "怀坏淮槐徊踝" },
    { "huan", "欢环换还缓幻患宦焕唤浣桓寰奂" },
    { "huang", "黄皇慌谎晃煌凰惶璜簧恍" },
    { "hui", "回会灰汇惠毁悔辉慧挥徽卉晦秽喙蕙" },
    { "hun", "昏婚浑混魂馄荤" },
    { "huo", "火货活伙或祸霍豁惑" },
    { "ji", "机几及己记季鸡基极吉急集际济剂击疾籍挤激缉脊棘冀姬绩缉羁畸箕矶赍犄" },
    { "jia", "家加价假架甲佳驾嫁贾嘉颊荚痂枷珈" },
    { "jian", "见间件建减兼坚简剪剑健舰监尖艰检荐渐践贱肩键箭笺缄菅犍" },
    { "jiang", "江将姜讲奖降浆疆匠蒋酱僵缰绛" },
    { "jiao", "交角教叫觉较脚娇骄焦蕉礁郊浇搅狡饺矫轿酵" },
    { "jie", "结解界接节街杰阶姐戒借捷阶睫芥疥讦婕" },
    { "jin", "金进近紧今斤尽禁锦津晋劲浸烬瑾槿" },
    { "jing", "京经精静境竟景井警镜敬径晶睛靖粳" },
    { "jiong", "炯窘迥冏" },
    { "jiu", "九就久酒旧救究纠舅灸玖韭臼厩" },
    { "ju", "居局举句巨具距锯聚据橘菊鞠疽驹掬莒" },
    { "juan", "卷圈倦卷鹃捐涓绢隽眷" },
    { "jue", "决觉绝爵诀掘倔撅攫桷珏" },
    { "jun", "军君均俊骏郡菌钧筠" },
    { "ka", "卡咖喀" },
    { "kai", "开凯楷慨凯铠垲" },
    { "kan", "看坎砍敢勘刊堪龛侃" },
    { "kang", "康抗扛炕慷糠亢" },
    { "kao", "考靠烤拷犒铐" },
    { "ke", "可克课科颗壳客渴刻坷苛珂嗑氪" },
    { "ken", "肯啃垦恳龈" },
    { "keng", "坑铿" },
    { "kong", "空孔恐控崆" },
    { "kou", "口扣寇叩抠" },
    { "ku", "苦库哭枯酷窟骷" },
    { "kua", "夸跨垮挎胯" },
    { "kuai", "快块筷脍侩哙" },
    { "kuan", "宽款髋" },
    { "kuang", "狂矿框筐况旷邝圹" },
    { "kui", "亏愧窥葵魁馈溃盔馗逵" },
    { "kun", "昆捆困坤鲲髡" },
    { "kuo", "扩阔括廓" },
    { "la", "拉啦辣蜡喇腊邋" },
    { "lai", "来赖莱籁睐" },
    { "lan", "兰蓝览烂栏篮揽兰阑谰澜褴" },
    { "lang", "郎狼浪廊朗琅榔稂" },
    { "lao", "老劳捞牢酪涝烙姥醪" },
    { "le", "乐勒了肋嘞" },
    { "lei", "雷类累泪垒蕾儡嘞嫘" },
    { "leng", "冷愣" },
    { "li", "里理力利离礼李立例丽历励吏黎梨璃篱莉俐锂栗溧" },
    { "lia", "俩" },
    { "lian", "连联练脸恋莲怜帘廉敛炼链涟鲢" },
    { "liang", "两量凉梁良粮亮辆靓晾" },
    { "liao", "了料辽疗聊燎廖僚撩嘹獠" },
    { "lie", "列烈劣猎裂冽咧鬣" },
    { "lin", "林临邻鳞凛赁琳霖嶙廪" },
    { "ling", "零灵玲领令岭龄凌铃陵菱棂" },
    { "liu", "流六留刘柳溜硫榴浏鎏" },
    { "long", "龙隆笼聋垄窿陇珑" },
    { "lou", "楼漏露陋娄搂喽镂" },
    { "lu", "路陆录卢鲁炉鹿禄卤虏麓庐泸" },
    { "lv", "驴旅绿缕屡律虑吕侣铝闾膂" },
    { "luan", "乱卵滦鸾" },
    { "luan", "乱卵滦鸾" },
    { "lue", "掠略" },
    { "lun", "论轮伦仑抡沦纶" },
    { "luo", "罗洛落络骆螺逻箩骡漯" },
    { "ma", "马吗嘛麻骂玛码蟆" },
    { "mai", "买卖麦迈脉埋霾" },
    { "man", "满慢瞒曼漫蔓蛮馒幔颟" },
    { "mang", "忙茫芒盲氓莽邙" },
    { "mao", "毛猫冒帽貌矛茂贸卯铆耄" },
    { "me", "么麽" },
    { "mei", "没美妹梅煤每味昧寐媚玫袂湄" },
    { "men", "门们闷焖懑" },
    { "meng", "梦孟蒙猛盟萌朦锰氓" },
    { "mi", "米密秘迷蜜眯弥糜泌幂咪祢" },
    { "mian", "面棉免勉眠绵冕娩腼眄" },
    { "miao", "苗秒妙描瞄藐庙喵" },
    { "mie", "灭蔑咩" },
    { "min", "民敏名皿闽泯悯敏岷" },
    { "ming", "明名命鸣铭冥茗溟" },
    { "miu", "谬缪" },
    { "mo", "莫摸磨墨末么膜魔摩抹茉谟馍" },
    { "mou", "某谋牟眸" },
    { "mu", "木目母亩牧墓幕慕暮穆沐睦钼" },
    { "na", "那哪拿娜纳呐捺钠" },
    { "nai", "奶耐奈乃萘" },
    { "nan", "南男难喃楠囡" },
    { "nang", "囊囔馕" },
    { "nao", "脑闹恼挠淖瑙" },
    { "ne", "呢呐" },
    { "nei", "内馁" },
    { "nen", "嫩" },
    { "neng", "能" },
    { "ni", "你泥尼拟逆倪妮霓昵腻" },
    { "nian", "年念粘粘拈蔫廿鲶" },
    { "niang", "娘酿" },
    { "niao", "鸟尿袅茑" },
    { "nie", "捏聂孽涅啮镊镍蹑" },
    { "nin", "您" },
    { "ning", "宁凝拧泞狞佞" },
    { "niu", "牛扭钮纽妞拗" },
    { "nong", "农浓弄脓" },
    { "nu", "奴努怒女弩胬" },
    { "nv", "女" },
    { "nuan", "暖" },
    { "nue", "虐疟" },
    { "nuo", "诺挪糯懦喏" },
    { "o", "哦喔窝蜗莪娥俄哦卧藕欧呕偶沤" },
    { "ou", "欧偶呕沤殴藕" },
    { "pa", "怕爬帕趴琶啪" },
    { "pai", "拍排派牌湃徘" },
    { "pan", "盘盼判攀畔潘番磐" },
    { "pang", "胖旁乓庞彷滂" },
    { "pao", "跑泡炮抛袍刨疱" },
    { "pei", "陪配培佩沛裴胚赔呸珮" },
    { "pen", "盆喷澎" },
    { "peng", "朋彭棚蓬碰捧烹澎膨鹏" },
    { "pi", "皮批匹脾疲屁譬劈坯毗琵貔痞郫" },
    { "pian", "片偏篇骗扁翩骈谝" },
    { "piao", "飘票漂瓢嫖剽" },
    { "pie", "撇瞥" },
    { "pin", "品频拼贫聘嫔" },
    { "ping", "平评瓶凭萍屏乒坪" },
    { "po", "破坡婆泼泊迫颇魄粕鄱" },
    { "pu", "普铺葡仆瀑浦蒲圃埔噗匍" },
    { "qi", "七其期起气奇器妻欺漆戚弃泣栖凄祁祈歧崎畦亓" },
    { "qia", "恰卡掐洽髂" },
    { "qian", "千前钱浅签迁铅谦牵钳乾虔黔" },
    { "qiang", "强墙抢枪腔呛蔷羟襁" },
    { "qiao", "桥乔巧俏侨翘窍撬樵荞" },
    { "qie", "切且怯窃茄砌妾惬锲" },
    { "qin", "亲秦琴勤禽寝沁钦芹芩" },
    { "qing", "青轻清情请庆卿晴氢氰擎" },
    { "qiong", "穷琼穹邛" },
    { "qiu", "秋求球丘邱囚酋裘蚯泅" },
    { "qu", "去取区屈曲趣娶趋渠蛆衢" },
    { "quan", "全泉权圈劝券拳颧铨" },
    { "que", "却缺确雀鹊瘸榷炔" },
    { "qun", "群裙逡" },
    { "ran", "然染燃冉苒" },
    { "rang", "让瓤嚷壤攘" },
    { "rao", "饶绕扰娆" },
    { "re", "热惹" },
    { "ren", "人仁忍任认刃妊纫壬稔" },
    { "reng", "仍扔" },
    { "ri", "日" },
    { "rong", "容荣融熔蓉榕茸冗戎" },
    { "rou", "肉柔揉蹂鞣" },
    { "ru", "如入儒汝乳辱茹蠕濡孺" },
    { "ruan", "软阮" },
    { "rui", "瑞锐蕊睿芮" },
    { "run", "润闰" },
    { "ruo", "若弱偌" },
    { "sa", "撒萨卅飒" },
    { "sai", "赛塞腮鳃" },
    { "san", "三散伞叁糁" },
    { "sang", "桑丧嗓搡" },
    { "sao", "扫嫂骚臊瘙" },
    { "se", "色塞瑟涩啬铯" },
    { "sen", "森" },
    { "seng", "僧" },
    { "sha", "杀沙纱啥傻煞厦霎鲨" },
    { "shai", "晒筛" },
    { "shan", "山善单衫扇闪陕珊删擅赡潸" },
    { "shang", "上商伤赏尚裳晌墒" },
    { "shao", "少烧绍邵勺哨梢韶劭" },
    { "she", "社设蛇舍射舌折涉奢赦佘" },
    { "shen", "身申神深沈审肾甚渗参娠砷" },
    { "sheng", "生声升圣胜绳省盛剩晟" },
    { "shi", "是时十石师史使事市室视实食识势世始式示士氏尸失诗狮匙屎驶拭噬" },
    { "shou", "手首受守寿售收授兽瘦狩" },
    { "shu", "书数树属叔舒输鼠术述束殊疏枢淑孰赎" },
    { "shua", "刷耍唰" },
    { "shuai", "帅率摔衰甩蟀" },
    { "shuan", "拴栓闩" },
    { "shuang", "双霜爽孀" },
    { "shui", "水睡税谁" },
    { "shun", "顺瞬舜吮" },
    { "shuo", "说硕朔烁妁铄" },
    { "si", "四死司思私丝斯撕寺肆嗣嘶蛳" },
    { "song", "送松宋诵颂耸讼嵩" },
    { "sou", "搜艘擞嗽叟嗾" },
    { "su", "素速苏诉肃宿俗塑酥稣" },
    { "suan", "酸算蒜" },
    { "sui", "岁碎随虽遂穗绥髓祟邃" },
    { "sun", "孙损笋荪榫" },
    { "suo", "所锁索缩嗦唆蓑娑" },
    { "ta", "他她它塔踏塌榻挞獭" },
    { "tai", "太台态胎泰抬苔酞汰" },
    { "tan", "谈坦叹探贪滩炭坛痰檀袒毯" },
    { "tang", "唐糖堂汤躺烫塘淌趟棠" },
    { "tao", "桃逃涛讨套陶淘萄韬" },
    { "te", "特忒忑" },
    { "teng", "疼腾藤誊" },
    { "ti", "体题提替梯踢蹄啼涕剃屉" },
    { "tian", "天田填甜舔腆佃恬" },
    { "tiao", "条跳调挑眺迢苕窕" },
    { "tie", "铁贴帖餮" },
    { "ting", "听厅停挺庭亭汀婷霆" },
    { "tong", "同通痛铜童桶筒桐彤仝" },
    { "tou", "头投透偷" },
    { "tu", "土图突途徒涂吐兔秃凸屠" },
    { "tuan", "团湍疃" },
    { "tui", "推退腿蜕颓兑" },
    { "tun", "吞屯臀褪豚" },
    { "tuo", "托脱妥拓唾陀驼椭沱拓" },
    { "wa", "瓦挖蛙洼娃袜娲" },
    { "wai", "外歪崴" },
    { "wan", "万完晚湾弯丸腕碗宛婉挽蔓烷" },
    { "wang", "王望网亡忘旺往妄汪罔" },
    { "wei", "为位味微围伟卫未伪尾威慰魏喂偎薇苇炜" },
    { "wen", "文问温闻稳纹吻蚊雯紊瘟" },
    { "weng", "翁嗡瓮蓊" },
    { "wo", "我窝卧握沃蜗斡" },
    { "wu", "呜无五吴武午舞雾乌污屋吾梧侮悟误物巫诬" },
    { "xi", "西希吸洗喜系细习戏席锡溪熙悉膝昔惜息曦" },
    { "xia", "下夏吓虾霞侠峡狭瑕厦" },
    { "xian", "先现线显县鲜闲仙贤弦险羡宪舷涎" },
    { "xiang", "想向香相祥乡享象像响襄湘厢镶" },
    { "xiao", "小笑肖消销晓萧孝效校嚣哮骁" },
    { "xie", "写谢解鞋斜协邪胁携屑懈械蟹" },
    { "xin", "心新信欣辛薪馨鑫昕衅" },
    { "xing", "行星兴姓形刑性醒幸杏腥惺" },
    { "xiong", "熊胸兄雄凶匈芎" },
    { "xiu", "修休秀锈袖臭宿羞溴" },
    { "xu", "许需须徐序绪续虚畜煦旭栩" },
    { "xuan", "选悬宣喧旋券玄眩绚煊萱" },
    { "xue", "学雪血穴靴薛削" },
    { "xun", "寻训讯迅熏勋循旬询浔殉" },
    { "ya", "牙呀雅亚压鸭崖丫衙涯" },
    { "yan", "言严沿盐炎颜眼演验烟咽厌宴艳燕彦檐" },
    { "yang", "阳杨洋样养氧央羊扬殃秧鸯" },
    { "yao", "要药姚摇遥窑妖腰咬耀尧瑶" },
    { "ye", "也叶夜业野爷耶冶液谒" },
    { "yi", "一伊衣医依仪宜夷移遗疑乙已以矣义亿艺易益议异意忆毅役逸" },
    { "yin", "因音银引饮隐印阴尹殷茵寅鄞" },
    { "ying", "英鹰应迎影硬映营莹颖樱莺萤萦" },
    { "yo", "哟" },
    { "yong", "用永勇拥庸泳咏踊俑壅臃" },
    { "you", "有又友优游由油邮犹悠幽诱佑釉" },
    { "yu", "于与雨语玉育域浴欲愈豫余鱼渔予娱郁遇喻御" },
    { "yuan", "元原员园圆远院愿怨渊源缘袁猿" },
    { "yue", "月越悦乐岳跃粤约阅玥" },
    { "yun", "云运晕允匀孕韵蕴耘郧" },
    { "za", "杂砸扎匝" },
    { "zai", "在再载崽灾宰哉" },
    { "zan", "赞暂咱攒簪" },
    { "zang", "脏葬藏赃" },
    { "zao", "早造澡噪躁遭枣灶凿皂" },
    { "ze", "则泽责择仄舴" },
    { "zei", "贼" },
    { "zen", "怎谮" },
    { "zeng", "增赠憎曾甑缯" },
    { "zha", "扎乍诈渣眨栅榨咋咤喳" },
    { "zhai", "摘宅债窄斋翟崽" },
    { "zhan", "占站展战粘瞻斩崭暂栈湛" },
    { "zhang", "张章长涨掌仗账胀障彰樟璋" },
    { "zhao", "找招照兆赵罩召诏肇" },
    { "zhe", "这者折哲浙蔗辙辄赭" },
    { "zhen", "真珍针震阵镇诊枕疹甄榛" },
    { "zheng", "正整证争征蒸睁挣郑政帧" },
    { "zhi", "之只知直制智治指纸志支枝织质置执职侄" },
    { "zhong", "中重众钟终种忠肿仲衷" },
    { "zhou", "周州舟洲咒宙肘帚皱纣" },
    { "zhu", "主住朱珠猪煮助注祝筑竹烛株诸嘱铢" },
    { "zhua", "抓爪" },
    { "zhuai", "拽" },
    { "zhuan", "专转传赚砖撰篆颛" },
    { "zhuang", "庄装状壮妆撞幢" },
    { "zhui", "追准坠椎赘缀" },
    { "zhun", "准谆" },
    { "zhuo", "捉着桌琢灼浊酌茁啄镯" },
    { "zi", "子字自资姿滋紫仔籽孜恣眦" },
    { "zong", "总宗棕踪纵鬃粽" },
    { "zou", "邹走奏揍邹" },
    { "zu", "租足卒族祖阻组卒做坐座左佐"},
    { "zuan", "钻纂攥钻" },
    { "zui", "嘴醉最罪" },
    { "zun", "尊遵樽" },
    { "zuo", "作做坐座左佐撮" },
    
    // LVGL遍历结束标记（必须保留）
    { NULL, NULL }
};
#endif

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
lv_obj_t * lv_ime_pinyin_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/*=====================
 * Setter functions
 *====================*/

void lv_ime_pinyin_set_keyboard(lv_obj_t * obj, lv_obj_t * kb)
{
    if(kb) {
        LV_ASSERT_OBJ(kb, &lv_keyboard_class);
    }

    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    pinyin_ime->kb = kb;
    lv_obj_set_parent(obj, lv_obj_get_parent(kb));
    lv_obj_set_parent(pinyin_ime->cand_panel, lv_obj_get_parent(kb));
    lv_obj_add_event_cb(pinyin_ime->kb, lv_ime_pinyin_kb_event, LV_EVENT_VALUE_CHANGED, obj);
    lv_obj_align_to(pinyin_ime->cand_panel, pinyin_ime->kb, LV_ALIGN_OUT_TOP_MID, 0, 0);
}

void lv_ime_pinyin_set_dict(lv_obj_t * obj, lv_pinyin_dict_t * dict)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    init_pinyin_dict(obj, dict);
}

void lv_ime_pinyin_set_mode(lv_obj_t * obj, lv_ime_pinyin_mode_t mode)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    LV_ASSERT_OBJ(pinyin_ime->kb, &lv_keyboard_class);

    pinyin_ime->mode = mode;

#if LV_IME_PINYIN_USE_K9_MODE
    if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K9) {
        pinyin_k9_init_data(obj);
        lv_keyboard_set_map(pinyin_ime->kb, LV_KEYBOARD_MODE_USER_1, (const char **)lv_btnm_def_pinyin_k9_map,
                            default_kb_ctrl_k9_map);
        lv_keyboard_set_mode(pinyin_ime->kb, LV_KEYBOARD_MODE_USER_1);
    }
#endif
}

/*=====================
 * Getter functions
 *====================*/

lv_obj_t * lv_ime_pinyin_get_kb(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    return pinyin_ime->kb;
}

lv_obj_t * lv_ime_pinyin_get_cand_panel(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    return pinyin_ime->cand_panel;
}

const lv_pinyin_dict_t * lv_ime_pinyin_get_dict(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    return pinyin_ime->dict;
}

/*=====================
 * Other functions
 *====================*/

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_ime_pinyin_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    uint16_t py_str_i = 0;
    uint16_t btnm_i = 0;
    for(btnm_i = 0; btnm_i < (LV_IME_PINYIN_CAND_TEXT_NUM + 3); btnm_i++) {
        if(btnm_i == 0) {
            lv_btnm_def_pinyin_sel_map[btnm_i] = "<";
        }
        else if(btnm_i == (LV_IME_PINYIN_CAND_TEXT_NUM + 1)) {
            lv_btnm_def_pinyin_sel_map[btnm_i] = ">";
        }
        else if(btnm_i == (LV_IME_PINYIN_CAND_TEXT_NUM + 2)) {
            lv_btnm_def_pinyin_sel_map[btnm_i] = "";
        }
        else {
            lv_pinyin_cand_str[py_str_i][0] = ' ';
            lv_btnm_def_pinyin_sel_map[btnm_i] = lv_pinyin_cand_str[py_str_i];
            py_str_i++;
        }
    }

    pinyin_ime->mode = LV_IME_PINYIN_MODE_K26;
    pinyin_ime->py_page = 0;
    pinyin_ime->ta_count = 0;
    pinyin_ime->cand_num = 0;
    lv_memzero(pinyin_ime->input_char, sizeof(pinyin_ime->input_char));
    lv_memzero(pinyin_ime->py_num, sizeof(pinyin_ime->py_num));
    lv_memzero(pinyin_ime->py_pos, sizeof(pinyin_ime->py_pos));

    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);

#if LV_IME_PINYIN_USE_DEFAULT_DICT
    init_pinyin_dict(obj, lv_ime_pinyin_def_dict);
#endif

    /* Init pinyin_ime->cand_panel */
    pinyin_ime->cand_panel = lv_buttonmatrix_create(lv_obj_get_parent(obj));
    lv_buttonmatrix_set_map(pinyin_ime->cand_panel, (const char **)lv_btnm_def_pinyin_sel_map);
    lv_obj_set_size(pinyin_ime->cand_panel, LV_PCT(100), LV_PCT(5));
    lv_obj_add_flag(pinyin_ime->cand_panel, LV_OBJ_FLAG_HIDDEN);

    lv_buttonmatrix_set_one_checked(pinyin_ime->cand_panel, true);
    lv_obj_remove_flag(pinyin_ime->cand_panel, LV_OBJ_FLAG_CLICK_FOCUSABLE);

    /* Set cand_panel style*/
    // Default style
    lv_obj_set_style_bg_opa(pinyin_ime->cand_panel, LV_OPA_0, 0);
    lv_obj_set_style_border_width(pinyin_ime->cand_panel, 0, 0);
    lv_obj_set_style_pad_all(pinyin_ime->cand_panel, 8, 0);
    lv_obj_set_style_pad_gap(pinyin_ime->cand_panel, 0, 0);
    lv_obj_set_style_radius(pinyin_ime->cand_panel, 0, 0);
    lv_obj_set_style_pad_gap(pinyin_ime->cand_panel, 0, 0);
    lv_obj_set_style_base_dir(pinyin_ime->cand_panel, LV_BASE_DIR_LTR, 0);

    // LV_PART_ITEMS style
    lv_obj_set_style_radius(pinyin_ime->cand_panel, 12, LV_PART_ITEMS);
    lv_obj_set_style_bg_color(pinyin_ime->cand_panel, lv_color_white(), LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(pinyin_ime->cand_panel, LV_OPA_0, LV_PART_ITEMS);
    lv_obj_set_style_shadow_opa(pinyin_ime->cand_panel, LV_OPA_0, LV_PART_ITEMS);

    // LV_PART_ITEMS | LV_STATE_PRESSED style
    lv_obj_set_style_bg_opa(pinyin_ime->cand_panel, LV_OPA_COVER, LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(pinyin_ime->cand_panel, lv_color_white(), LV_PART_ITEMS | LV_STATE_PRESSED);

    /* event handler */
    lv_obj_add_event_cb(pinyin_ime->cand_panel, lv_ime_pinyin_cand_panel_event, LV_EVENT_VALUE_CHANGED, obj);
    lv_obj_add_event_cb(obj, lv_ime_pinyin_style_change_event, LV_EVENT_STYLE_CHANGED, NULL);

#if LV_IME_PINYIN_USE_K9_MODE
    pinyin_ime->k9_input_str_len = 0;
    pinyin_ime->k9_py_ll_pos = 0;
    pinyin_ime->k9_legal_py_count = 0;
    lv_memzero(pinyin_ime->k9_input_str, LV_IME_PINYIN_K9_MAX_INPUT);

    pinyin_k9_init_data(obj);

    lv_ll_init(&(pinyin_ime->k9_legal_py_ll), sizeof(ime_pinyin_k9_py_str_t));
#endif
}

static void lv_ime_pinyin_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    if(lv_obj_is_valid(pinyin_ime->kb))
        lv_obj_delete(pinyin_ime->kb);

    if(lv_obj_is_valid(pinyin_ime->cand_panel))
        lv_obj_delete(pinyin_ime->cand_panel);
}

static void lv_ime_pinyin_kb_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * kb = lv_event_get_current_target(e);
    lv_obj_t * obj = lv_event_get_user_data(e);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

#if LV_IME_PINYIN_USE_K9_MODE
    static const char * k9_py_map[8] = {"abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
#endif

    if(code == LV_EVENT_VALUE_CHANGED) {
        uint16_t btn_id  = lv_buttonmatrix_get_selected_button(kb);
        if(btn_id == LV_BUTTONMATRIX_BUTTON_NONE) return;

        const char * txt = lv_buttonmatrix_get_button_text(kb, lv_buttonmatrix_get_selected_button(kb));
        if(txt == NULL) return;

        lv_obj_t * ta = lv_keyboard_get_textarea(pinyin_ime->kb);

#if LV_IME_PINYIN_USE_K9_MODE
        if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K9) {

            uint16_t tmp_button_str_len = lv_strlen(pinyin_ime->input_char);
            if((btn_id >= 16) && (tmp_button_str_len > 0) && (btn_id < (16 + LV_IME_PINYIN_K9_CAND_TEXT_NUM))) {
                lv_memzero(pinyin_ime->input_char, sizeof(pinyin_ime->input_char));
                lv_strcat(pinyin_ime->input_char, txt);
                pinyin_input_proc(obj);

                for(int index = 0; index < (pinyin_ime->ta_count + tmp_button_str_len); index++) {
                    lv_textarea_delete_char(ta);
                }

                pinyin_ime->ta_count = tmp_button_str_len;
                pinyin_ime->k9_input_str_len = tmp_button_str_len;
                lv_textarea_add_text(ta, pinyin_ime->input_char);

                return;
            }
        }
#endif

        if(lv_strcmp(txt, "Enter") == 0 || lv_strcmp(txt, LV_SYMBOL_NEW_LINE) == 0) {
            pinyin_ime_clear_data(obj);
            lv_obj_add_flag(pinyin_ime->cand_panel, LV_OBJ_FLAG_HIDDEN);
        }
        else if(lv_strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) {
            // del input char
            if(pinyin_ime->ta_count > 0) {
                if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K26)
                    pinyin_ime->input_char[pinyin_ime->ta_count - 1] = '\0';
#if LV_IME_PINYIN_USE_K9_MODE
                else
                    pinyin_ime->k9_input_str[pinyin_ime->ta_count - 1] = '\0';
#endif

                pinyin_ime->ta_count--;
                if(pinyin_ime->ta_count <= 0) {
                    pinyin_ime_clear_data(obj);
                    lv_obj_add_flag(pinyin_ime->cand_panel, LV_OBJ_FLAG_HIDDEN);
                }
                else if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K26) {
                    pinyin_input_proc(obj);
                }
#if LV_IME_PINYIN_USE_K9_MODE
                else if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K9) {
                    pinyin_ime->k9_input_str_len = lv_strlen(pinyin_ime->input_char) - 1;
                    pinyin_k9_get_legal_py(obj, pinyin_ime->k9_input_str, k9_py_map);
                    pinyin_k9_fill_cand(obj);
                    pinyin_input_proc(obj);
                    pinyin_ime->ta_count--;
                }
#endif
            }
        }
        else if((lv_strcmp(txt, "ABC") == 0) || (lv_strcmp(txt, "abc") == 0) || (lv_strcmp(txt, "1#") == 0) ||
                (lv_strcmp(txt, LV_SYMBOL_OK) == 0)) {
            pinyin_ime_clear_data(obj);
            return;
        }
        else if(lv_strcmp(txt, "123") == 0) {
            for(uint16_t i = 0; i < lv_strlen(txt); i++)
                lv_textarea_delete_char(ta);

            pinyin_ime_clear_data(obj);
            lv_textarea_set_cursor_pos(ta, LV_TEXTAREA_CURSOR_LAST);
            lv_ime_pinyin_set_mode(obj, LV_IME_PINYIN_MODE_K9_NUMBER);
            lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
            lv_obj_add_flag(pinyin_ime->cand_panel, LV_OBJ_FLAG_HIDDEN);
        }
        else if(lv_strcmp(txt, LV_SYMBOL_KEYBOARD) == 0) {
            if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K26) {
                lv_ime_pinyin_set_mode(obj, LV_IME_PINYIN_MODE_K9);
            }
            else if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K9) {
                lv_ime_pinyin_set_mode(obj, LV_IME_PINYIN_MODE_K26);
                lv_keyboard_set_mode(pinyin_ime->kb, LV_KEYBOARD_MODE_TEXT_LOWER);
            }
            else if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K9_NUMBER) {
                lv_ime_pinyin_set_mode(obj, LV_IME_PINYIN_MODE_K9);
            }
            pinyin_ime_clear_data(obj);
        }
        else if((pinyin_ime->mode == LV_IME_PINYIN_MODE_K26) && ((txt[0] >= 'a' && txt[0] <= 'z') || (txt[0] >= 'A' &&
                                                                                                      txt[0] <= 'Z'))) {
            uint16_t len = lv_strlen(pinyin_ime->input_char);
            lv_snprintf(pinyin_ime->input_char + len, sizeof(pinyin_ime->input_char) - len, "%s", txt);
            pinyin_input_proc(obj);
            pinyin_ime->ta_count++;
        }
#if LV_IME_PINYIN_USE_K9_MODE
        else if((pinyin_ime->mode == LV_IME_PINYIN_MODE_K9) && (txt[0] >= 'a' && txt[0] <= 'z')) {
            for(uint16_t i = 0; i < 8; i++) {
                if((lv_strcmp(txt, k9_py_map[i]) == 0) || (lv_strcmp(txt, "abc ") == 0)) {
                    if(lv_strcmp(txt, "abc ") == 0)    pinyin_ime->k9_input_str_len += lv_strlen(k9_py_map[i]) + 1;
                    else                            pinyin_ime->k9_input_str_len += lv_strlen(k9_py_map[i]);
                    pinyin_ime->k9_input_str[pinyin_ime->ta_count] = 50 + i;
                    pinyin_ime->k9_input_str[pinyin_ime->ta_count + 1] = '\0';

                    break;
                }
            }
            pinyin_k9_get_legal_py(obj, pinyin_ime->k9_input_str, k9_py_map);
            pinyin_k9_fill_cand(obj);
            pinyin_input_proc(obj);
        }
        else if(lv_strcmp(txt, LV_SYMBOL_LEFT) == 0) {
            pinyin_k9_cand_page_proc(obj, 0);
        }
        else if(lv_strcmp(txt, LV_SYMBOL_RIGHT) == 0) {
            pinyin_k9_cand_page_proc(obj, 1);
        }
#endif
    }
}

static void lv_ime_pinyin_cand_panel_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * cand_panel = lv_event_get_current_target(e);
    lv_obj_t * obj = (lv_obj_t *)lv_event_get_user_data(e);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    if(code == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t * ta = lv_keyboard_get_textarea(pinyin_ime->kb);
        if(ta == NULL) return;

        uint32_t id = lv_buttonmatrix_get_selected_button(cand_panel);
        if(id == LV_BUTTONMATRIX_BUTTON_NONE) {
            return;
        }
        else if(id == 0) {
            pinyin_page_proc(obj, 0);
            return;
        }
        else if(id == (LV_IME_PINYIN_CAND_TEXT_NUM + 1)) {
            pinyin_page_proc(obj, 1);
            return;
        }

        const char * txt = lv_buttonmatrix_get_button_text(cand_panel, id);
        uint16_t index = 0;
        for(index = 0; index < pinyin_ime->ta_count; index++)
            lv_textarea_delete_char(ta);

        lv_textarea_add_text(ta, txt);

        pinyin_ime_clear_data(obj);
    }
}

static void pinyin_input_proc(lv_obj_t * obj)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    pinyin_ime->cand_str = pinyin_search_matching(obj, pinyin_ime->input_char, &pinyin_ime->cand_num);
    if(pinyin_ime->cand_str == NULL) {
        return;
    }

    pinyin_ime->py_page = 0;

    for(uint8_t i = 0; i < LV_IME_PINYIN_CAND_TEXT_NUM; i++) {
        lv_memset(lv_pinyin_cand_str[i], 0x00, sizeof(lv_pinyin_cand_str[i]));
        lv_pinyin_cand_str[i][0] = ' ';
    }

    // fill buf
    for(uint8_t i = 0; (i < pinyin_ime->cand_num && i < LV_IME_PINYIN_CAND_TEXT_NUM); i++) {
        for(uint8_t j = 0; j < 3; j++) {
            lv_pinyin_cand_str[i][j] = pinyin_ime->cand_str[i * 3 + j];
        }
    }

    lv_obj_remove_flag(pinyin_ime->cand_panel, LV_OBJ_FLAG_HIDDEN);
}

static void pinyin_page_proc(lv_obj_t * obj, uint16_t dir)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;
    uint16_t page_num = pinyin_ime->cand_num / LV_IME_PINYIN_CAND_TEXT_NUM;
    uint16_t remainder = pinyin_ime->cand_num % LV_IME_PINYIN_CAND_TEXT_NUM;

    if(!pinyin_ime->cand_str) return;

    if(dir == 0) {
        if(pinyin_ime->py_page) {
            pinyin_ime->py_page--;
        }
    }
    else {
        if(remainder == 0) {
            page_num -= 1;
        }
        if(pinyin_ime->py_page < page_num) {
            pinyin_ime->py_page++;
        }
        else return;
    }

    for(uint8_t i = 0; i < LV_IME_PINYIN_CAND_TEXT_NUM; i++) {
        lv_memset(lv_pinyin_cand_str[i], 0x00, sizeof(lv_pinyin_cand_str[i]));
        lv_pinyin_cand_str[i][0] = ' ';
    }

    // fill buf
    uint16_t offset = pinyin_ime->py_page * (3 * LV_IME_PINYIN_CAND_TEXT_NUM);
    for(uint8_t i = 0; (i < pinyin_ime->cand_num && i < LV_IME_PINYIN_CAND_TEXT_NUM); i++) {
        if((remainder > 0) && (pinyin_ime->py_page == page_num)) {
            if(i >= remainder)
                break;
        }
        for(uint8_t j = 0; j < 3; j++) {
            lv_pinyin_cand_str[i][j] = pinyin_ime->cand_str[offset + (i * 3) + j];
        }
    }
}

static void lv_ime_pinyin_style_change_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    if(code == LV_EVENT_STYLE_CHANGED) {
        const lv_font_t * font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
        lv_obj_set_style_text_font(pinyin_ime->cand_panel, font, 0);
    }
}

static void init_pinyin_dict(lv_obj_t * obj, const lv_pinyin_dict_t * dict)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    char headletter = 'a';
    uint16_t offset_sum = 0;
    uint16_t offset_count = 0;
    uint16_t letter_calc = 0;

    pinyin_ime->dict = dict;

    for(uint16_t i = 0; ; i++) {
        if((NULL == (dict[i].py)) || (NULL == (dict[i].py_mb))) {
            headletter = dict[i - 1].py[0];
            letter_calc = headletter - 'a';
            pinyin_ime->py_num[letter_calc] = offset_count;
            break;
        }

        if(headletter == (dict[i].py[0])) {
            offset_count++;
        }
        else {
            headletter = dict[i].py[0];
            pinyin_ime->py_num[letter_calc] = offset_count;
            letter_calc = headletter - 'a';
            offset_sum += offset_count;
            pinyin_ime->py_pos[letter_calc] = offset_sum;

            offset_count = 1;
        }
    }
}

static char * pinyin_search_matching(lv_obj_t * obj, char * py_str, uint16_t * cand_num)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    const lv_pinyin_dict_t * cpHZ;
    uint8_t index, len = 0, offset;
    volatile uint8_t count = 0;

    if(*py_str == '\0')    return NULL;
    if(*py_str == 'i')     return NULL;
    if(*py_str == 'u')     return NULL;
    if(*py_str == 'v')     return NULL;
    if(*py_str == ' ')     return NULL;

    offset = py_str[0] - 'a';
    len = lv_strlen(py_str);

    cpHZ  = &pinyin_ime->dict[pinyin_ime->py_pos[offset]];
    count = pinyin_ime->py_num[offset];

    while(count--) {
        for(index = 0; index < len; index++) {
            if(*(py_str + index) != *((cpHZ->py) + index)) {
                break;
            }
        }

        // perfect match
        if(len == 1 || index == len) {
            // The Chinese character in UTF-8 encoding format is 3 bytes
            * cand_num = lv_strlen((const char *)(cpHZ->py_mb)) / 3;
            return (char *)(cpHZ->py_mb);
        }
        cpHZ++;
    }
    return NULL;
}

static void pinyin_ime_clear_data(lv_obj_t * obj)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

#if LV_IME_PINYIN_USE_K9_MODE
    if(pinyin_ime->mode == LV_IME_PINYIN_MODE_K9) {
        pinyin_ime->k9_input_str_len = 0;
        pinyin_ime->k9_py_ll_pos = 0;
        pinyin_ime->k9_legal_py_count = 0;
        lv_memzero(pinyin_ime->k9_input_str,  LV_IME_PINYIN_K9_MAX_INPUT);
        lv_memzero(lv_pinyin_k9_cand_str, sizeof(lv_pinyin_k9_cand_str));
        for(uint8_t i = 0; i < LV_IME_PINYIN_CAND_TEXT_NUM; i++) {
            lv_strcpy(lv_pinyin_k9_cand_str[i], " ");
        }
        lv_strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM], LV_SYMBOL_RIGHT"\0");
        lv_strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 1], "\0");
        lv_buttonmatrix_set_map(pinyin_ime->kb, (const char **)lv_btnm_def_pinyin_k9_map);
    }
#endif

    pinyin_ime->ta_count = 0;
    for(uint8_t i = 0; i < LV_IME_PINYIN_CAND_TEXT_NUM; i++) {
        lv_memset(lv_pinyin_cand_str[i], 0x00, sizeof(lv_pinyin_cand_str[i]));
        lv_pinyin_cand_str[i][0] = ' ';
    }
    lv_memzero(pinyin_ime->input_char, sizeof(pinyin_ime->input_char));

    lv_obj_add_flag(pinyin_ime->cand_panel, LV_OBJ_FLAG_HIDDEN);
}

#if LV_IME_PINYIN_USE_K9_MODE
static void pinyin_k9_init_data(lv_obj_t * obj)
{
    LV_UNUSED(obj);

    uint16_t py_str_i = 0;
    uint16_t btnm_i = 0;
    for(btnm_i = 19; btnm_i < (LV_IME_PINYIN_K9_CAND_TEXT_NUM + 21); btnm_i++) {
        if(py_str_i == LV_IME_PINYIN_K9_CAND_TEXT_NUM) {
            lv_strcpy(lv_pinyin_k9_cand_str[py_str_i], LV_SYMBOL_RIGHT"\0");
        }
        else if(py_str_i == LV_IME_PINYIN_K9_CAND_TEXT_NUM + 1) {
            lv_strcpy(lv_pinyin_k9_cand_str[py_str_i], "\0");
        }
        else {
            lv_strcpy(lv_pinyin_k9_cand_str[py_str_i], " \0");
        }

        lv_btnm_def_pinyin_k9_map[btnm_i] = lv_pinyin_k9_cand_str[py_str_i];
        py_str_i++;
    }

    default_kb_ctrl_k9_map[0]  = LV_BUTTONMATRIX_CTRL_CHECKED | 1;
    default_kb_ctrl_k9_map[1]  = LV_BUTTONMATRIX_CTRL_NO_REPEAT | LV_BUTTONMATRIX_CTRL_CLICK_TRIG | 1;
    default_kb_ctrl_k9_map[4]  = LV_BUTTONMATRIX_CTRL_CHECKED | 1;
    default_kb_ctrl_k9_map[5]  = LV_BUTTONMATRIX_CTRL_CHECKED | 1;
    default_kb_ctrl_k9_map[9]  = LV_KEYBOARD_CTRL_BUTTON_FLAGS | 1;
    default_kb_ctrl_k9_map[10] = LV_BUTTONMATRIX_CTRL_CHECKED | 1;
    default_kb_ctrl_k9_map[14] = LV_BUTTONMATRIX_CTRL_CHECKED | 1;
    default_kb_ctrl_k9_map[15] = LV_BUTTONMATRIX_CTRL_CHECKED | 1;
    default_kb_ctrl_k9_map[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 16] = LV_BUTTONMATRIX_CTRL_CHECKED | 1;
}

static void pinyin_k9_get_legal_py(lv_obj_t * obj, char * k9_input, const char * py9_map[])
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    uint16_t len = lv_strlen(k9_input);

    if((len == 0) || (len >= LV_IME_PINYIN_K9_MAX_INPUT)) {
        return;
    }

    char py_comp[LV_IME_PINYIN_K9_MAX_INPUT] = {0};
    int mark[LV_IME_PINYIN_K9_MAX_INPUT] = {0};
    int index = 0;
    int flag = 0;
    uint16_t count = 0;

    uint32_t ll_len = 0;
    ime_pinyin_k9_py_str_t * ll_index = NULL;

    ll_len = lv_ll_get_len(&pinyin_ime->k9_legal_py_ll);
    ll_index = lv_ll_get_head(&pinyin_ime->k9_legal_py_ll);

    while(index != -1) {
        if(index == len) {
            if(pinyin_k9_is_valid_py(obj, py_comp)) {
                if((count >= ll_len) || (ll_len == 0)) {
                    ll_index = lv_ll_ins_tail(&pinyin_ime->k9_legal_py_ll);
                    lv_strcpy(ll_index->py_str, py_comp);
                }
                else if((count < ll_len)) {
                    lv_strcpy(ll_index->py_str, py_comp);
                    ll_index = lv_ll_get_next(&pinyin_ime->k9_legal_py_ll, ll_index);
                }
                count++;
            }
            index--;
        }
        else {
            flag = mark[index];
            if((size_t)flag < lv_strlen(py9_map[k9_input[index] - '2'])) {
                py_comp[index] = py9_map[k9_input[index] - '2'][flag];
                mark[index] = mark[index] + 1;
                index++;
            }
            else {
                mark[index] = 0;
                index--;
            }
        }
    }

    if(count > 0) {
        pinyin_ime->ta_count++;
        pinyin_ime->k9_legal_py_count = count;
    }
}

/*true: visible; false: not visible*/
static bool pinyin_k9_is_valid_py(lv_obj_t * obj, char * py_str)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    const lv_pinyin_dict_t * cpHZ = NULL;
    uint8_t index = 0, len = 0, offset = 0;
    volatile uint8_t count = 0;

    if(*py_str == '\0')    return false;
    if(*py_str == 'i')     return false;
    if(*py_str == 'u')     return false;
    if(*py_str == 'v')     return false;

    offset = py_str[0] - 'a';
    len = lv_strlen(py_str);

    cpHZ  = &pinyin_ime->dict[pinyin_ime->py_pos[offset]];
    count = pinyin_ime->py_num[offset];

    while(count--) {
        for(index = 0; index < len; index++) {
            if(*(py_str + index) != *((cpHZ->py) + index)) {
                break;
            }
        }

        // perfect match
        if(len == 1 || index == len) {
            return true;
        }
        cpHZ++;
    }
    return false;
}

static void pinyin_k9_fill_cand(lv_obj_t * obj)
{
    uint16_t index = 0, tmp_len = 0;
    ime_pinyin_k9_py_str_t * ll_index = NULL;

    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    tmp_len = pinyin_ime->k9_legal_py_count;

    if(tmp_len != cand_len) {
        lv_memzero(lv_pinyin_k9_cand_str, sizeof(lv_pinyin_k9_cand_str));
        lv_strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM], LV_SYMBOL_RIGHT"\0");
        lv_strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 1], "\0");
        cand_len = tmp_len;
    }

    ll_index = lv_ll_get_head(&pinyin_ime->k9_legal_py_ll);
    lv_strcpy(pinyin_ime->input_char, ll_index->py_str);

    for(uint8_t i = 0; i < LV_IME_PINYIN_K9_CAND_TEXT_NUM; i++) {
        lv_strcpy(lv_pinyin_k9_cand_str[i], " ");
    }

    while(ll_index) {
        if(index >= LV_IME_PINYIN_K9_CAND_TEXT_NUM)
            break;

        if(index < pinyin_ime->k9_legal_py_count) {
            lv_strcpy(lv_pinyin_k9_cand_str[index], ll_index->py_str);
        }

        ll_index = lv_ll_get_next(&pinyin_ime->k9_legal_py_ll, ll_index); /*Find the next list*/
        index++;
    }
    pinyin_ime->k9_py_ll_pos = index;

    lv_obj_t * ta = lv_keyboard_get_textarea(pinyin_ime->kb);
    for(index = 0; index < pinyin_ime->k9_input_str_len; index++) {
        lv_textarea_delete_char(ta);
    }
    pinyin_ime->k9_input_str_len = lv_strlen(pinyin_ime->input_char);
    lv_textarea_add_text(ta, pinyin_ime->input_char);
}

static void pinyin_k9_cand_page_proc(lv_obj_t * obj, uint16_t dir)
{
    lv_ime_pinyin_t * pinyin_ime = (lv_ime_pinyin_t *)obj;

    lv_obj_t * ta = lv_keyboard_get_textarea(pinyin_ime->kb);
    uint16_t ll_len =  lv_ll_get_len(&pinyin_ime->k9_legal_py_ll);

    if((ll_len > LV_IME_PINYIN_K9_CAND_TEXT_NUM) && (pinyin_ime->k9_legal_py_count > LV_IME_PINYIN_K9_CAND_TEXT_NUM)) {
        ime_pinyin_k9_py_str_t * ll_index = NULL;
        int count = 0;

        ll_index = lv_ll_get_head(&pinyin_ime->k9_legal_py_ll);
        while(ll_index) {
            if(count >= pinyin_ime->k9_py_ll_pos)   break;

            ll_index = lv_ll_get_next(&pinyin_ime->k9_legal_py_ll, ll_index); /*Find the next list*/
            count++;
        }

        if((NULL == ll_index) && (dir == 1))   return;

        lv_memzero(lv_pinyin_k9_cand_str, sizeof(lv_pinyin_k9_cand_str));
        lv_strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM], LV_SYMBOL_RIGHT"\0");
        lv_strcpy(lv_pinyin_k9_cand_str[LV_IME_PINYIN_K9_CAND_TEXT_NUM + 1], "\0");

        // next page
        if(dir == 1) {
            for(uint8_t i = 0; i < LV_IME_PINYIN_K9_CAND_TEXT_NUM; i++) {
                lv_strcpy(lv_pinyin_k9_cand_str[i], " ");
            }

            count = 0;
            while(ll_index) {
                if(count >= (LV_IME_PINYIN_K9_CAND_TEXT_NUM - 1))
                    break;

                lv_strcpy(lv_pinyin_k9_cand_str[count], ll_index->py_str);
                ll_index = lv_ll_get_next(&pinyin_ime->k9_legal_py_ll, ll_index); /*Find the next list*/
                count++;
            }
            pinyin_ime->k9_py_ll_pos += count - 1;

        }
        // previous page
        else {
            for(uint8_t i = 0; i < LV_IME_PINYIN_K9_CAND_TEXT_NUM; i++) {
                lv_strcpy(lv_pinyin_k9_cand_str[i], " ");
            }
            count = LV_IME_PINYIN_K9_CAND_TEXT_NUM - 1;
            ll_index = lv_ll_get_prev(&pinyin_ime->k9_legal_py_ll, ll_index);
            while(ll_index) {
                if(count < 0)  break;

                lv_strcpy(lv_pinyin_k9_cand_str[count], ll_index->py_str);
                ll_index = lv_ll_get_prev(&pinyin_ime->k9_legal_py_ll, ll_index); /*Find the previous list*/
                count--;
            }

            if(pinyin_ime->k9_py_ll_pos > LV_IME_PINYIN_K9_CAND_TEXT_NUM)
                pinyin_ime->k9_py_ll_pos -= 1;
        }

        lv_textarea_set_cursor_pos(ta, LV_TEXTAREA_CURSOR_LAST);
    }
}

#endif  /*LV_IME_PINYIN_USE_K9_MODE*/

#endif  /*LV_USE_IME_PINYIN*/
