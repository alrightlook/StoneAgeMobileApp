#ifndef _ANIM_TBL_H_
#define _ANIM_TBL_H_

#include "../anim_tbl_set.h"

/*ʯ��������ר������ͼ�� �ӣԣ��ң�*/

#define CG_DROPWND				35322   //26195ͼ����	 ��������ȷ��

#define CG_PKSERVER_BTN_UP		26193	// ѡȡ��ϵ����ťUp
#define CG_PKSERVER_BTN_DOWN	26194	// ѡȡ��ϵ����ťDown
#define CG_STATUS_DETAIL		26387	// ״̬��ϸť
#define CG_AUCWND_DETAIL		26381	// ������ϸ�Ӵ�
#define CG_AUCWND_MESSAGE		26382	// ����ѶϢ�Ӵ�
#define CG_AUCWND_COLUMN		26383	// ����ר��
#define CG_AUCWND_CHANGE		26384	// ������Ӵ�
#define CG_AUCWND_DETAIL2		26385	// ������ϸ�Ӵ�2

#define CG_BATTLE_ATT_DOWN		26361	// ս������ťDown
#define CG_BATTLE_ATT_UP		26362	// ս������ťUp
#define CG_BATTLE_HELP_DOWN		26363	// ս�����ťDown
#define CG_BATTLE_HELP_UP		26364	// ս�����ťUp
#define CG_BATTLE_DEF_DOWN		26365	// ս������ťDown
#define CG_BATTLE_DEF_UP		26366	// ս������ťUp
#define CG_BATTLE_JUJUTSU_DOWN	26367	// ս������ťDown
#define CG_BATTLE_JUJUTSU_UP	26368	// ս������ťUp
#define CG_BATTLE_PANEL			26369	// ս���װ�
#define CG_BATTLE_ESCAPE_DOWN	26370	// ս������ťDown
#define CG_BATTLE_ESCAPE_UP		26371	// ս������ťUp
#define CG_BATTLE_PET_DOWN		26372	// ս������ťDown
#define CG_BATTLE_PET_UP		26373	// ս������ťUp
#define CG_BATTLE_CAPTURE_DOWN	26374	// ս����׽ťDown
#define CG_BATTLE_CAPTURE_UP	26375	// ս����׽ťUp
#define CG_BATTLE_ITEM_DOWN		26376	// ս������ťDown
#define CG_BATTLE_ITEM_UP		26377	// ս������ťUp
#define CG_BATTLE_PROSKL_DOWN	26378	// ս������ťDown
#define CG_BATTLE_PROSKL_UP		26379	// ս������ťUp

#define CG_FIELD_SKILL_TILE		26354	// ְҵ���ܵװ�
#define CG_FIELD_SKILL_ADVSK	26355	// ְҵ���ܽ�����׼���ť
#define CG_FIELD_SKILL_BATTLE	26357	// ְҵ���ܽ���ս������ť
#define CG_FIELD_SKILL_ASSIT	26356	// ְҵ���ܽ��渨������ť

#define CG_FIELD_SKILL_MENU		26358	// �����ϰ�ť���
#define CG_FIELD_SKILL_BTN_OFF	26360	// ְҵ����ťOFF
#define CG_FIELD_SKILL_BTN_ON	26361	// ְҵ����ťON

#ifdef _FRIENDCHANNEL			//ROG ADD ����Ƶ��
#define CG_FIELD_CHATROOM_BTN_ON         26453   //���������ϰ�ť
#define CG_FIELD_CHATROOM_BTN_OFF        26454   //���������ϰ�ť
#define CG_FIELD_CLOSE_BTN_UP			 26444   //�����ҹرհ�ť
#define CG_FIELD_CLOSE_BTN_DOWN			 26445   //�����ҹرհ�ť
#define CG_FIELD_CHANGECHIEF_BTN_UP		 26435   //�����Ҹ����ҳ���ť
#define CG_FIELD_CHANGECHIEF_BTN_DOWN	 26436   //�����Ҹ����ҳ���ť
#define CG_FIELD_CHANGECHIEF_BTN_DISABLE 26437   //�����Ҹ����ҳ���ť
#define CG_FIELD_OUTMEMBER_BTN_UP        26432   //����������
#define CG_FIELD_OUTMEMBER_BTN_DOWN      26433   //����������
#define CG_FIELD_OUTMEMBER_BTN_DISABLE   26434   //����������
#define CG_FIELD_LEAVE_BTN_UP			 26441   //�����뿪��ť
#define CG_FIELD_LEAVE_BTN_DOWN			 26442   //�����뿪��ť
#define CG_FIELD_DELETE_BTN_UP			 26438	 //ɾ��������
#define CG_FIELD_DELETE_BTN_DOWN		 26439	 //ɾ��������
#define CG_FIELD_DELETE_BTN_DISABLE		 26440	 //ɾ��������
#define CG_FIELD_AGREE_BTN				 26091   //����ͬ�ⰴť
#define CG_FIELD_DISAGREE_BTN			 26092   //����񶨰�ť
#define CG_FIELD_CHANNEL_MENU			 26452	 //�����ϰ�ť���
#define CG_FIELD_CHANNEL_SCROLL			 26447   //����
#define CG_FIELD_SCROLL_HUP				 26448   //����
#define CG_FIELD_SCROLL_HDOWN			 26449   //����
#define CG_FIELD_SCROLL_LUP			     26450   //����
#define CG_FIELD_SCROLL_LDOWN			 26451   //����
#endif

#ifdef _STREET_VENDOR
#define CG_FIELD_SV_BTN_OFF							35227		// ���ϰ�̯���ܰ�����(��)
#define CG_FIELD_SV_BTN_ON							35226		// ���ϰ�̯���ܰ�����(��)
#define CG_FIELD_SV_BUY_PANEL						35240		// ��̯����(��)
#endif

#ifdef _TEACHER_SYSTEM
#define CG_FIELD_TS_BTN_OFF							28555		// �����е�ʦ���ܰ�����(��)
#define CG_FIELD_TS_BTN_ON							28554		// �����е�ʦ���ܰ�����(��)
#define CG_FIELD_TS_LEFT_MENU						28553
#endif

#ifdef	_TIMEBAR_FUNCTION                        //ʱ��BAR��ʽ
#define CG_TIMEBAR_PANEL                 26350   //ʱ��bar�Ŀ�
#define CG_TIMEBAR_UNIT					 26428   //ʱ�����ĵ�λ
#endif

#ifdef _NEWPANEL //Syu ADD 7.0 ������״̬����
#define CG_NEW_STATUS_SKILL_BTN_UP		26312
#define	CG_SKILL_TABLE					26380
#define CG_WAR_ICON_BIG					29281
#define CG_WIZ_ICON_BIG					29282
#define CG_HUNTER_ICON_BIG				29283
#define CG_WAR_ICON_SMALL				29284
#define CG_WIZ_ICON_SMALL				29285
#define CG_HUNTER_ICON_SMALL			29286
#endif

//#define CG_NEWITEM_WND			26349	// ����װ����λ�Ӵ�(���֡���)
#ifdef _TRADESYSTEM2	// (���ɿ�) Syu ADD �½���ϵͳ
#define CG_TRADE_SEAL			26330	// ����ӡ��
#define CG_TRADE_SCROLL_UP		26331	// ���׾����ϼ�ͷ
#define CG_TRADE_SCROLL_DOWN	26332	// ���׾����¼�ͷ
#define CG_TRADE_SCROLL_BTN		26333	// ���׾�����ҷť
#define CG_TRADE_DEF_LINE		26334	// �����Ӵ�������
#define CG_TRADE_LV_LINE		26335	// �����Ӵ��ȼ���
#define CG_TRADE_LINE			26336	// �����Ӵ�������
#define CG_TRADE_DEX_LINE		26337	// �����Ӵ��ٶ���
#define CG_TRADE_ATK_LINE		26338	// �����Ӵ�������
#define CG_TRADE_SK1_LINE		26339	// �����Ӵ����ܣ�
#define CG_TRADE_SK2_LINE		26340	// �����Ӵ����ܣ�
#define CG_TRADE_SK3_LINE		26341	// �����Ӵ����ܣ�
#define CG_TRADE_SK4_LINE		26342	// �����Ӵ����ܣ�	
#define CG_TRADE_SK5_LINE		26343	// �����Ӵ����ܣ�
#define CG_TRADE_SK6_LINE		26344	// �����Ӵ����ܣ�
#define CG_TRADE_SK7_LINE		26345	// �����Ӵ����ܣ�
#define CG_TRADE_HP_LINE		26346	// �����Ӵ�Ѫ����
#define CG_TRADE_VIEW_BTN		26347	// �����Ӵ�����ť
#define CG_TRADE_BACK_BTN		26348	// �����Ӵ�����ť
#define CG_TRADE_TALK_WND		26350	// ���׶Ի��Ӵ�
#endif

/*
#define CG_BATTLE_ANGRY			26521
#define CG_BATTLE_CRAZY			26520
#define CG_BATTLE_GUARD			26519
#define CG_BATTLE_ATTPET		26518
#define CG_BATTLE_FORGOT		26517
#define CG_BATTLE_CAPTURE_UP	26516
*/
/*
#define SPR_bloodbug			101623	//��Ѫ�� ( Wiz ) 
#define SPR_bthunder			101624	//������ ( Wiz ) 
#define SPR_bugnet				101625	//ѱ������ ( Hunter )
#define SPR_lightning			101626  //�������� ( War )
#define SPR_redsmoke			101627	//������ ( ���� ) 
#define SPR_sthunder			101628	//������ ( Wiz ) 
#define SPR_track				101629	//׷Ѱ���� ( Hunter ) 
#define SPR_trap				101630  //���� ( Hunter ) 
#define SPR_shieldattack		101631	//�ܻ� ( War ) ������
#define SPR_shieldattack2		101632	//�ܻ� ( War ) �����ҷ�
#define SPR_bloodsucker			101633	//һ���Ѫ ( Wiz ) 
#define SPR_bstorm				101634  //������ �� ( Wiz ) �����ұ�
#define SPR_bstorm2				101635	//������ �� ( Wiz ) �������
#define SPR_concentrate			101636	//רעս�� ( War ) 
#define SPR_deadattach			101637	//�������� ( War )
#define SPR_dodge				101638	//�ر� ( War ) 
#define SPR_doom				101639  //����ĩ�� ( Wiz ) �����ҷ�
#define SPR_doom2				101640  //����ĩ�� ( Wiz ) ������
#define SPR_dragonbreath		101641	//����ǹ ( Wiz ) ������
#define SPR_dragonbreath2		101642	//����ǹ ( Wiz ) �����ҷ�
#define SPR_encloseghost		101643	//������ ( Wiz ) �����ҷ�
#define SPR_encloseghost2		101644	//������ ( Wiz ) ������
#define SPR_energycollect		101645	//�����ۼ� ( War ) 
#define SPR_fireresist1			101646	//���濹�� ( Hunter ) �ų�
#define SPR_fireresist2			101647	//���濹�� ( Hunter ) ��
#define SPR_icearrow			101648	//������ ( Wiz ) ������
#define SPR_icearrow2			101649	//������ ( Wiz ) �����ҷ�
#define SPR_iceexplode			101650  //������ ( Wiz ) �����ҷ�
#define SPR_iceexplode2			101651  //������ ( Wiz ) ������
#define SPR_icemirror			101652	//������ ( Wiz ) 
#define SPR_iceresist1			101653	//�������� ( Hunter ) �ų�
#define SPR_iceresist2			101654	//�������� ( Hunter ) ��
#define SPR_lgyration			101655	//�������� ( War ) ��
#define SPR_lgyration1			101656	//�������� ( War ) ����
#define SPR_lgyration2			101657	//�������� ( War ) ǰ��
#define SPR_lightresist1		101658	//���翹�� ( Hunter ) �ų�
#define SPR_lightresist2		101659	//���翹�� ( Hunter ) ��
#define SPR_lpentrate1			101660	//�ᴩ���� ( War ) �����ҷ��ķ�
#define SPR_lpentrate2			101661	//�ᴩ���� ( War )
#define SPR_lpentrate3			101662	//�ᴩ���� ( War )
#define SPR_lpentrate4			101663	//�ᴩ���� ( War )
#define SPR_lpentrate5			101664	//�ᴩ���� ( War )
#define SPR_lpentrate6			101665	//�ᴩ���� ( War )
#define SPR_poisonresist1		101666	//�綾���� ( Hunter ) �ų�
#define SPR_poisonresist2		101667	//�綾���� ( Hunter ) ��
#define SPR_rgyration			101668	//�������� ( War ) 
#define SPR_rgyration1			101669  //�������� ( War ) 
#define SPR_rgyration2			101670  //�������� ( War ) 
#define SPR_rpentrate1			101671	//�ᴩ���� ( War ) �����󷽵ķ�
#define SPR_rpentrate2			101672	//�ᴩ���� ( War ) 
#define SPR_rpentrate3			101673	//�ᴩ���� ( War ) 
#define SPR_rpentrate4			101674	//�ᴩ���� ( War ) 
#define SPR_rpentrate5			101675	//�ᴩ���� ( War ) 
#define SPR_rpentrate6			101676	//�ᴩ���� ( War ) 
#define SPR_storm				101677	//����ѩ ( Wiz ) �����ҷ�
#define SPR_storm2				101678	//����ѩ ( Wiz ) ������
#define SPR_violent				101679	//�������� ( War ) 
#define SPR_waterdrop			101680	//ˮ��״̬
#define SPR_weakness			101681	//���㹥�� ( Hunter ) 
#define SPR_root				101682	//��������	( Hunter ) 
#define SPR_net					101683	//���޵���	( Hunter ) 
#define SPR_boomattack			101684  //����		( War ) 
#define SPR_superthunder		101685	//������ Lv10 ( Wiz )
#define SPR_volspring1			101686	//��ɽȪ ��	 ( Wiz ) 
#define SPR_volspring2			101687	//��ɽȪ ��	 ( Wiz ) 
#define SPR_volspring3			101688	//��ɽȪ ��	 ( Wiz ) 
#define SPR_bloodthirsty1		101689	//��Ѫ���� Lv10 ( Wiz ) 
#define SPR_bloodthirsty2		101690	//��Ѫ���� Lv7	( Wiz ) 
#define SPR_bloodthirsty3		101691	//��Ѫ���� Lv4	( Wiz ) 
#define SPR_bloodthirsty4		101692	//��Ѫ���� Lv1	( Wiz ) 
#define SPR_lfireplanet			101693	//������	( Wiz ) ������
#define SPR_rfireplanet			101694	//������	( Wiz ) �����ҷ�
#define SPR_tree				101695	//���λ�λ	( Wiz ) 
#define SPR_trap1				101696  //����
#define SPR_wizuse				101697	//��ʦʹ��
#define SPR_hunteruse			101698	//����ʹ��
#define SPR_fireattack			101699	//���幥��
#define SPR_iceattack			101700	//�����幥��
#define SPR_lightattack			101701	//�׸��幥��
101705	ˮ����
101706	�纣��
101707	ˮ�𺣲�
101708	��غ���
101709	��ط���
101710	������
101711	ˮ��׾�
101712	��ˮ�׾�
101713	��ش���è
101714	������è
101715	��ˮ����   ( δ���� )
101716	��纣��   ( δ���� )
101717  �����սʿ 
101718  ˮ����սʿ
101719  ���С��è
101720  ˮ��С��è
101721  �����ͷ�� 
101722  �����ͷ�� 
101723  ���ɡ���� ( δ���� )
101724  ��ˮɡ���� ( δ���� )
101725	ʨ�ӵ�ˮ  
101726  ʨ��ˮ��  
101727	ʨ�ӻ��  
101728	ʨ�ӷ��  
101729	����	 
*/
/*ʯ��������ר������ͼ�� �ţΣ� */



#define SPRSTART			100000		// SPR????��???�k?
// ???
#define SPR_001em			100000		//	��?�i?	����
#define SPR_001ax			100001		// 			?
#define SPR_001cl			100002		//			??�S
#define SPR_001sp			100003		//			?
#define SPR_001bw			100004		//			?

#define SPR_002em			100005		//	��?�i?	����
#define SPR_002ax			100006		// 			?
#define SPR_002cl			100007		//			??�S
#define SPR_002sp			100008		//			?
#define SPR_002bw			100009		//			?

#define SPR_003em			100010		//	��?�i?	����
#define SPR_003ax			100011		// 			?
#define SPR_003cl			100012		//			??�S
#define SPR_003sp			100013		//			?
#define SPR_003bw			100014		//			?

#define SPR_004em			100015		//	��?�i?	����
#define SPR_004ax			100016		// 			?
#define SPR_004cl			100017		//			??�S
#define SPR_004sp			100018		//			?
#define SPR_004bw			100019		//			?

#define SPR_011em			100020		//	��g?		����
#define SPR_011ax			100021		// 			?
#define SPR_011cl			100022		//			??�S
#define SPR_011sp			100023		//			?
#define SPR_011bw			100024		//			?

#define SPR_012em			100025		//	��g?		����
#define SPR_012ax			100026		// 			?
#define SPR_012cl			100027		//			??�S
#define SPR_012sp			100028		//			?
#define SPR_012bw			100029		//			?

#define SPR_013em			100030		//	��g?		����
#define SPR_013ax			100031		// 			?
#define SPR_013cl			100032		//			??�S
#define SPR_013sp			100033		//			?
#define SPR_013bw			100034		//			?

#define SPR_014em			100035		//	��g?		����
#define SPR_014ax			100036		// 			?
#define SPR_014cl			100037		//			??�S
#define SPR_014sp			100038		//			?
#define SPR_014bw			100039		//			?

#define SPR_021em			100040		//	��g?		����
#define SPR_021ax			100041		// 			?
#define SPR_021cl			100042		//			??�S
#define SPR_021sp			100043		//			?
#define SPR_021bw			100044		//			?

#define SPR_022em			100045
#define SPR_022ax			100046
#define SPR_022cl			100047
#define SPR_022sp			100048
#define SPR_022bw			100049

#define SPR_023em			100050
#define SPR_023ax			100051
#define SPR_023cl			100052
#define SPR_023sp			100053
#define SPR_023bw			100054

#define SPR_024em			100055
#define SPR_024ax			100056
#define SPR_024cl			100057
#define SPR_024sp			100058
#define SPR_024bw			100059

#define SPR_031em			100060		//	��g?		����
#define SPR_031ax			100061		// 			?
#define SPR_031cl			100062		//			??�S
#define SPR_031sp			100063		//			?
#define SPR_031bw			100064		//			?

#define SPR_032em			100065
#define SPR_032ax			100066
#define SPR_032cl			100067
#define SPR_032sp			100068
#define SPR_032bw			100069

#define SPR_033em			100070
#define SPR_033ax			100071
#define SPR_033cl			100072
#define SPR_033sp			100073
#define SPR_033bw			100074

#define SPR_034em			100075
#define SPR_034ax			100076
#define SPR_034cl			100077
#define SPR_034sp			100078
#define SPR_034bw			100079

#define SPR_041em			100080		//	��g?		����
#define SPR_041ax			100081		// 			?
#define SPR_041cl			100082		//			??�S
#define SPR_041sp			100083		//			?
#define SPR_041bw			100084		//			?

#define SPR_042em			100085
#define SPR_042ax			100086
#define SPR_042cl			100087
#define SPR_042sp			100088
#define SPR_042bw			100089

#define SPR_043em			100090
#define SPR_043ax			100091
#define SPR_043cl			100092
#define SPR_043sp			100093
#define SPR_043bw			100094

#define SPR_044em			100095
#define SPR_044ax			100096
#define SPR_044cl			100097
#define SPR_044sp			100098
#define SPR_044bw			100099

#define SPR_051em			100100		//	��g?		����
#define SPR_051ax			100101		// 			?
#define SPR_051cl			100102		//			??�S
#define SPR_051sp			100103		//			?
#define SPR_051bw			100104		//			?

#define SPR_052em			100105
#define SPR_052ax			100106
#define SPR_052cl			100107
#define SPR_052sp			100108
#define SPR_052bw			100109

#define SPR_053em			100110
#define SPR_053ax			100111
#define SPR_053cl			100112
#define SPR_053sp			100113
#define SPR_053bw			100114

#define SPR_054em			100115
#define SPR_054ax			100116
#define SPR_054cl			100117
#define SPR_054sp			100118
#define SPR_054bw			100119

#define SPR_061em			100120		//	��???		����
#define SPR_061ax			100121		// 			?
#define SPR_061cl			100122		//			??�S
#define SPR_061sp			100123		//			?
#define SPR_061bw			100124		//			?

#define SPR_062em			100125
#define SPR_062ax			100126
#define SPR_062cl			100127
#define SPR_062sp			100128
#define SPR_062bw			100129

#define SPR_063em			100130
#define SPR_063ax			100131
#define SPR_063cl			100132
#define SPR_063sp			100133
#define SPR_063bw			100134

#define SPR_064em			100135
#define SPR_064ax			100136
#define SPR_064cl			100137
#define SPR_064sp			100138
#define SPR_064bw			100139

#define SPR_071em			100140		//	��???	����
#define SPR_071ax			100141		// 			?
#define SPR_071cl			100142		//			??�S
#define SPR_071sp			100143		//			?
#define SPR_071bw			100144		//			?

#define SPR_072em			100145
#define SPR_072ax			100146
#define SPR_072cl			100147
#define SPR_072sp			100148
#define SPR_072bw			100149

#define SPR_073em			100150
#define SPR_073ax			100151
#define SPR_073cl			100152
#define SPR_073sp			100153
#define SPR_073bw			100154

#define SPR_074em			100155
#define SPR_074ax			100156
#define SPR_074cl			100157
#define SPR_074sp			100158
#define SPR_074bw			100159

#define SPR_081em			100160		//	��??		����
#define SPR_081ax			100161		// 			?
#define SPR_081cl			100162		//			??�S
#define SPR_081sp			100163		//			?
#define SPR_081bw			100164		//			?

#define SPR_082em			100165
#define SPR_082ax			100166
#define SPR_082cl			100167
#define SPR_082sp			100168
#define SPR_082bw			100169

#define SPR_083em			100170
#define SPR_083ax			100171
#define SPR_083cl			100172
#define SPR_083sp			100173
#define SPR_083bw			100174

#define SPR_084em			100175
#define SPR_084ax			100176
#define SPR_084cl			100177
#define SPR_084sp			100178
#define SPR_084bw			100179

#define SPR_091em			100180		//	��??		����
#define SPR_091ax			100181		// 			?
#define SPR_091cl			100182		//			??�S
#define SPR_091sp			100183		//			?
#define SPR_091bw			100184		//			?

#define SPR_092em			100185
#define SPR_092ax			100186
#define SPR_092cl			100187
#define SPR_092sp			100188
#define SPR_092bw			100189

#define SPR_093em			100190
#define SPR_093ax			100191
#define SPR_093cl			100192
#define SPR_093sp			100193
#define SPR_093bw			100194

#define SPR_094em			100195
#define SPR_094ax			100196
#define SPR_094cl			100197
#define SPR_094sp			100198
#define SPR_094bw			100199

#define SPR_101em			100200		//	?��?		����
#define SPR_101ax			100201		// 			?
#define SPR_101cl			100202		//			??�S
#define SPR_101sp			100203		//			?
#define SPR_101bw			100204		//			?

#define SPR_102em			100205
#define SPR_102ax			100206
#define SPR_102cl			100207
#define SPR_102sp			100208
#define SPR_102bw			100209

#define SPR_103em			100210
#define SPR_103ax			100211
#define SPR_103cl			100212
#define SPR_103sp			100213
#define SPR_103bw			100214

#define SPR_104em			100215
#define SPR_104ax			100216
#define SPR_104cl			100217
#define SPR_104sp			100218
#define SPR_104bw			100219

#define SPR_111em			100220		//	?��?		����
#define SPR_111ax			100221		// 			?
#define SPR_111cl			100222		//			??�S
#define SPR_111sp			100223		//			?
#define SPR_111bw			100224		//			?

#define SPR_112em			100225
#define SPR_112ax			100226
#define SPR_112cl			100227
#define SPR_112sp			100228
#define SPR_112bw			100229

#define SPR_113em			100230
#define SPR_113ax			100231
#define SPR_113cl			100232
#define SPR_113sp			100233
#define SPR_113bw			100234

#define SPR_114em			100235
#define SPR_114ax			100236
#define SPR_114cl			100237
#define SPR_114sp			100238
#define SPR_114bw			100239


// ????��????
#define SPR_pet001			100250		//	???
#define SPR_pet002			100251		//	????
#define SPR_pet003			100252		//	??????
#define SPR_pet004			100253		//	????

#define SPR_pet011			100254		//	????
#define SPR_pet012			100255		//	???????
#define SPR_pet013			100256		//	???
#define SPR_pet014			100257		//	?????
#define SPR_pet015			100258		//	????

#define SPR_pet021			100259		//	????
#define SPR_pet022			100260		//	????
#define SPR_pet023			100261		//	????
#define SPR_pet024			100262		//	????

#define SPR_pet031			100263		//	???
#define SPR_pet032			100264		//	????
#define SPR_pet033			100265		//	????
#define SPR_pet034			100266		//	????

#define SPR_pet041			100267		//	????
#define SPR_pet042			100268		//	?????
#define SPR_pet043			100269		//	??????
#define SPR_pet044			100270		//	????

#define SPR_pet051			100271		//	?????
#define SPR_pet052			100272		//	???
#define SPR_pet053			100273		//	????
#define SPR_pet054			100274		//	????

#define SPR_pet061			100275		//	???
#define SPR_pet062			100276		//	????
#define SPR_pet063			100277		//	????
#define SPR_pet064			100278		//	????

#define SPR_pet071			100279		//	????
#define SPR_pet072			100280		//	???
#define SPR_pet073			100281		//	?????
#define SPR_pet074			100282		//	????

#define SPR_pet081			100283		//	????
#define SPR_pet082			100284		//	?????
#define SPR_pet083			100285		//	????
#define SPR_pet084			100286		//	?????

#define SPR_pet091			100287		//	?????
#define SPR_pet092			100288		//	?????
#define SPR_pet093			100289		//	?????
#define SPR_pet094			100290		//	?????

#define SPR_pet101			100291		//	???
#define SPR_pet102			100292		//	???
#define SPR_pet103			100293		//	????
#define SPR_pet104			100294		//	????

#define SPR_pet111			100295		//	????
#define SPR_pet112			100296		//	????
#define SPR_pet113			100297		//	????
#define SPR_pet114			100298		//	?????

#define SPR_pet121			100299		//	??
#define SPR_pet122			100300		//	??
#define SPR_pet123			100301		//	??
#define SPR_pet124			100302		//	??

#define SPR_pet131			100303		//	??
#define SPR_pet132			100304		//	??
#define SPR_pet133			100305		//	??
#define SPR_pet134			100306		//	??

#define SPR_pet141			100307		//	????
#define SPR_pet142			100308		//	????
#define SPR_pet143			100309		//	??????
#define SPR_pet144			100310		//	????

#define SPR_pet151			100311		//	??
#define SPR_pet152			100312		//	??
#define SPR_pet153			100313		//	??
#define SPR_pet154			100314		//	??

#define SPR_pet161			100315		//	??
#define SPR_pet162			100316		//	??
#define SPR_pet163			100317		//	??
#define SPR_pet164			100318		//	??

#define SPR_pet171			100319		//	????
#define SPR_pet172			100320		//	????
#define SPR_pet173			100321		//	????
#define SPR_pet174			100322		//	?????

#define SPR_pet181			100323		//	????
#define SPR_pet182			100324		//	????
#define SPR_pet183			100325		//	???
#define SPR_pet184			100326		//	????

#define SPR_pet191			100327		//	????
#define SPR_pet192			100328		//	?????
#define SPR_pet193			100329		//	????
#define SPR_pet194			100330		//	???????

#define SPR_pet201			100331		//	??
#define SPR_pet202			100332		//	??
#define SPR_pet203			100333		//	??
#define SPR_pet204			100334		//	??

#define SPR_pet211			100335		//	?????
#define SPR_pet212			100336		//	????
#define SPR_pet213			100337		//	?????
#define SPR_pet214			100338		//	????

#define SPR_pet221			100339		//	?????
#define SPR_pet222			100340		//	????
#define SPR_pet223			100341		//	????
#define SPR_pet224			100342		//	????

#define SPR_pet231			100343		//	????
#define SPR_pet232			100344		//	???
#define SPR_pet233			100345		//	?????
#define SPR_pet234			100346		//	?????

#define SPR_pet241			100347		//	????
#define SPR_pet242			100348		//	????
#define SPR_pet243			100349		//	????
#define SPR_pet244			100350		//	?????

#define SPR_pet251			100351		//	?????
#define SPR_pet252			100352		//	?????
#define SPR_pet253			100353		//	?????
#define SPR_pet254			100354		//	?????

#define SPR_pet261			100355		//	????
#define SPR_pet262			100356		//	???
#define SPR_pet263			100357		//	????
#define SPR_pet264			100358		//	?????

#define SPR_pet271			100359		//	????
#define SPR_pet272			100360		//	????
#define SPR_pet273			100361		//	?????
#define SPR_pet274			100362		//	????

#define SPR_pet281			100363		//	??
#define SPR_pet282			100364		//	??
#define SPR_pet283			100365		//	??
#define SPR_pet284			100366		//	??

#define SPR_pet291			100367		//	????
#define SPR_pet292			100368		//	????
#define SPR_pet293			100369		//	????
#define SPR_pet294			100370		//	?????

#define SPR_pet301			100371		//	????
#define SPR_pet302			100372		//	?????
#define SPR_pet303			100373		//	????
#define SPR_pet304			100374		//	?????

#define SPR_pet311			100375		//	???
#define SPR_pet312			100376		//	?????
#define SPR_pet313			100377		//	?????
#define SPR_pet314			100378		//	?????

#define SPR_pet321			100379		//	??????
#define SPR_pet322			100380		//	????
#define SPR_pet323			100381		//	?????
#define SPR_pet324			100382		//	?????
#define SPR_pet325			100383		//	????

#define SPR_pet331			100384		//	?????
#define SPR_pet332			100385		//	?????
#define SPR_pet333			100386		//	?????
#define SPR_pet334			100387		//	??????

// ��?�D????
#define SPR_pet005			100388		//	??
#define SPR_pet085			100389		//	??????
#define SPR_pet086			100390		//	��????
#define SPR_pet087			100391		//	?????????
#define SPR_pet088			100392		//	??????????
#define SPR_pet065			100393		//	?�N��???
#define SPR_pet095			100394		//	?�N��?????
#define SPR_pet175			100395		//	?�N��????
#define SPR_pet255			100396		//	?�N��?????

// ��?�D?????????
#define SPR_121em			100400		//	????	����
#define SPR_121ax			100401		//			?
#define SPR_121cl			100402		//			?�S
#define SPR_121sp			100403		//			?
#define SPR_121bw			100404		//			?

#define SPR_122em			100405		//	????	����
#define SPR_122ax			100406		//			?
#define SPR_122cl			100407		//			?�S
#define SPR_122sp			100408		//			?
#define SPR_122bw			100409		//			?

#define SPR_123em			100410		//	????	����
#define SPR_123ax			100411		//			?
#define SPR_123cl			100412		//			?�S
#define SPR_123sp			100413		//			?
#define SPR_123bw			100414		//			?

#define SPR_124em			100415		//	????	����
#define SPR_124ax			100416		//			?
#define SPR_124cl			100417		//			?�S
#define SPR_124sp			100418		//			?
#define SPR_124bw			100419		//			?

#define SPR_131em			100420		//	????	����
#define SPR_131ax			100421		//			?
#define SPR_131cl			100422		//			?�S
#define SPR_131sp			100423		//			?
#define SPR_131bw			100424		//			?

#define SPR_132em			100425		//	????	����
#define SPR_132ax			100426		//			?
#define SPR_132cl			100427		//			?�S
#define SPR_132sp			100428		//			?
#define SPR_132bw			100429		//			?

#define SPR_pet315			100430		//	??????

#define SPR_141em			100431		//	?????	����
#define SPR_141ax			100432		//			?
#define SPR_141cl			100433		//			?�S
#define SPR_141sp			100434		//			?
#define SPR_141bw			100435		//			?

#define SPR_142em			100436		//	?????	����
#define SPR_142ax			100437		//			?
#define SPR_142cl			100438		//			?�S
#define SPR_142sp			100439		//			?
#define SPR_142bw			100440		//			?

#define SPR_133em			100441		//	????	����
#define SPR_133ax			100442		//			?
#define SPR_133cl			100443		//			?�S
#define SPR_133sp			100444		//			?
#define SPR_133bw			100445		//			?

#define SPR_151em			100446		//	��???	����
#define SPR_151ax			100447		// 			?
#define SPR_151cl			100448		//			??�S
#define SPR_151sp			100449		//			?
#define SPR_151bw			100450		//			?

#define SPR_152em			100451
#define SPR_152ax			100452
#define SPR_152cl			100453
#define SPR_152sp			100454
#define SPR_152bw			100455

#define SPR_161em			100456		//	��???	����
#define SPR_161ax			100457		// 			?
#define SPR_161cl			100458		//			??�S
#define SPR_161sp			100459		//			?
#define SPR_161bw			100460		//			?

#define SPR_162em			100461
#define SPR_162ax			100462
#define SPR_162cl			100463
#define SPR_162sp			100464
#define SPR_162bw			100465

//?  ??  ��?????
#define SPR_mwood1			100466
#define SPR_mwood2			100467
#define SPR_mwood3			100468
#define SPR_mstone1			100469
#define SPR_mstone2			100470
#define SPR_mstone3			100471

//?��???????
#define SPR_leader			100500		//	??????????????
#define SPR_star			100501		//	?????��??????
#define SPR_boomerang		100502		//	?????????????
#define SPR_stonebomb		100503		//	��??????????
#define SPR_ono				100504		//	??????????
#define SPR_onokage			100505		//	?????????????
#define SPR_isiware			100506		//	��?????????
#define SPR_mail			100507		//	????��???????
//?????
#define SPR_stone			100550		//	��??????
#define SPR_shock			100551		//	???????
#define SPR_drunk			100552		//	???????
#define SPR_sleep			100553		//	��??????
#define SPR_conf			100554		//	???????
#define SPR_poison			100555		//	��?????
#define SPR_zokusei			100556		//	�N�ͨ�??????
#ifdef _MAGIC_WEAKEN
#define	SPR_weaken          101420	//����	
#endif
#ifdef _MAGIC_DEEPPOISION
#define	SPR_deeppoison      101417	//�綾	
#endif
#ifdef _MAGIC_BARRIER
#define	SPR_barrier         101421	//ħ��
#endif
#ifdef _MAGIC_NOCAST
#define	SPR_nocast          101419	//��Ĭ
#endif

//����ǿ��
#define	SPR_all_att			101406	//ȫ����
#define	SPR_land_att		101403	//��
#define	SPR_water_att		101404	//ˮ
#define	SPR_fire_att		101402	//��
#define	SPR_wind_att		101405	//��
//����ת��
//#define	SPR_all_att			101406	//ȫ����
#define	SPR_ch_earth		101408	//��
#define	SPR_ch_water		101409	//ˮ
#define	SPR_ch_fire		101407	//��
#define	SPR_ch_wind		101410	//��

#define	SPR_tooth			101415	//����
#define	SPR_mic_def			101411	//ħ������
#define	SPR_ironwall		101416	//����
#define	SPR_bomb            101418  //�Ա�

#ifdef _MIND_ICON
#define	SPR_heart           101296
#define	SPR_broken          101297
#define	SPR_m_clip          101300
#define	SPR_m_stone         101301
#define	SPR_m_burlap        101302
#define	SPR_screamer        101299
#define	SPR_question_mark   101289
#define	SPR_cramp           101292
#define	SPR_black_squall    101293
#define	SPR_perspiration    101298
#define	SPR_silence         101291
#define	SPR_asleep          101290
#define	SPR_cafe            101294
#define	SPR_lightbulb       101295
#define	SPR_accent          101288
#define	SPR_money           101303
#define SPR_pk				101304
#endif
//����????????????
#define SPR_effect01		100600		//	����???????
//#define SPR_heal			100601		//	����????????????
#define SPR_heal			100601		//	����????????????
#define SPR_heal2			100602		//	����????????????
#define SPR_heal3			100603		//	����????????????
#define SPR_tyusya			100604		//	����??????????????��???
#define SPR_hoshi			100605		//	����????????????????
#define SPR_kyu				100606		//	???????����????????????????
#define SPR_fukkatu1		100607		//	?????????????????????
#define SPR_fukkatu2		100608		//	?????????????????????
#define SPR_fukkatu3		100609		//	?????????????????????
#define SPR_difence			100610		//	�\??����????????????????
#define SPR_item			100611		//	?????��?????????????
#define SPR_item3			100612		//	?????��?????????????
//�\??����???????
#define SPR_mirror			100650		//	�T��???????
#define SPR_barrior			100651		//	??????????
#define SPR_kyusyu			100652		//	?��????????????

#define SPR_005em			100700
#define SPR_005ax			100701
#define SPR_005cl			100702
#define SPR_005sp			100703
#define SPR_005bw			100704

#define SPR_006em			100705
#define SPR_006ax			100706
#define SPR_006cl			100707
#define SPR_006sp			100708
#define SPR_006bw			100709

#define SPR_015em			100710
#define SPR_015ax			100711
#define SPR_015cl			100712
#define SPR_015sp			100713
#define SPR_015bw			100714

#define SPR_016em			100715
#define SPR_016ax			100716
#define SPR_016cl			100717
#define SPR_016sp			100718
#define SPR_016bw			100719

#define SPR_025em			100720
#define SPR_025ax			100721
#define SPR_025cl			100722
#define SPR_025sp			100723
#define SPR_025bw			100724

#define SPR_026em			100725
#define SPR_026ax			100726
#define SPR_026cl			100727
#define SPR_026sp			100728
#define SPR_026bw			100729

#define SPR_035em			100730
#define SPR_035ax			100731
#define SPR_035cl			100732
#define SPR_035sp			100733
#define SPR_035bw			100734

#define SPR_036em			100735
#define SPR_036ax			100736
#define SPR_036cl			100737
#define SPR_036sp			100738
#define SPR_036bw			100739

#define SPR_045em			100740
#define SPR_045ax			100741
#define SPR_045cl			100742
#define SPR_045sp			100743
#define SPR_045bw			100744

#define SPR_046em			100745
#define SPR_046ax			100746
#define SPR_046cl			100747
#define SPR_046sp			100748
#define SPR_046bw			100749

#define SPR_055em			100750
#define SPR_055ax			100751
#define SPR_055cl			100752
#define SPR_055sp			100753
#define SPR_055bw			100754

#define SPR_056em			100755
#define SPR_056ax			100756
#define SPR_056cl			100757
#define SPR_056sp			100758
#define SPR_056bw			100759

#define SPR_065em			100760
#define SPR_065ax			100761
#define SPR_065cl			100762
#define SPR_065sp			100763
#define SPR_065bw			100764

#define SPR_066em			100765
#define SPR_066ax			100766
#define SPR_066cl			100767
#define SPR_066sp			100768
#define SPR_066bw			100769

#define SPR_075em			100770
#define SPR_075ax			100771
#define SPR_075cl			100772
#define SPR_075sp			100773
#define SPR_075bw			100774

#define SPR_076em			100775
#define SPR_076ax			100776
#define SPR_076cl			100777
#define SPR_076sp			100778
#define SPR_076bw			100779

#define SPR_085em			100780
#define SPR_085ax			100781
#define SPR_085cl			100782
#define SPR_085sp			100783
#define SPR_085bw			100784

#define SPR_086em			100785
#define SPR_086ax			100786
#define SPR_086cl			100787
#define SPR_086sp			100788
#define SPR_086bw			100789

#define SPR_095em			100790
#define SPR_095ax			100791
#define SPR_095cl			100792
#define SPR_095sp			100793
#define SPR_095bw			100794

#define SPR_096em			100795
#define SPR_096ax			100796
#define SPR_096cl			100797
#define SPR_096sp			100798
#define SPR_096bw			100799

#define SPR_105em			100800
#define SPR_105ax			100801
#define SPR_105cl			100802
#define SPR_105sp			100803
#define SPR_105bw			100804

#define SPR_106em			100805
#define SPR_106ax			100806
#define SPR_106cl			100807
#define SPR_106sp			100808
#define SPR_106bw			100809

#define SPR_115em			100810
#define SPR_115ax			100811
#define SPR_115cl			100812
#define SPR_115sp			100813
#define SPR_115bw			100814

#define SPR_116em			100815
#define SPR_116ax			100816
#define SPR_116cl			100817
#define SPR_116sp			100818
#define SPR_116bw			100819



#define SPR_pet006			100820
#define SPR_pet007			100821
#define SPR_pet008			100822

#define SPR_pet016			100823
#define SPR_pet017			100824
#define SPR_pet018			100825

#define SPR_pet025			100826
#define SPR_pet026			100827
#define SPR_pet027			100828
#define SPR_pet028			100829

#define SPR_pet035			100830
#define SPR_pet036			100831
#define SPR_pet037			100832
#define SPR_pet038			100833

#define SPR_pet045			100834
#define SPR_pet046			100835
#define SPR_pet047			100836
#define SPR_pet048			100837

#define SPR_pet055			100838
#define SPR_pet056			100839
#define SPR_pet057			100840
#define SPR_pet058			100841

#define SPR_pet066			100842
#define SPR_pet067			100843
#define SPR_pet068			100844

#define SPR_pet075			100845
#define SPR_pet076			100846
#define SPR_pet077			100847
#define SPR_pet078			100848

#define SPR_pet096			100849
#define SPR_pet097			100850
#define SPR_pet098			100851

#define SPR_pet105			100852
#define SPR_pet106			100853
#define SPR_pet107			100854
#define SPR_pet108			100855

#define SPR_pet115			100856
#define SPR_pet116			100857
#define SPR_pet117			100858
#define SPR_pet118			100859

#define SPR_pet145			100860
#define SPR_pet146			100861
#define SPR_pet147			100862
#define SPR_pet148			100863

#define SPR_pet176			100864
#define SPR_pet177			100865
#define SPR_pet178			100866

#define SPR_pet185			100867
#define SPR_pet186			100868
#define SPR_pet187			100869
#define SPR_pet188			100870

#define SPR_pet195			100871
#define SPR_pet196			100872
#define SPR_pet197			100873
#define SPR_pet198			100874

#define SPR_pet215			100875
#define SPR_pet216			100876
#define SPR_pet217			100877
#define SPR_pet218			100878

#define SPR_pet225			100879
#define SPR_pet226			100880
#define SPR_pet227			100881
#define SPR_pet228			100882

#define SPR_pet235			100883
#define SPR_pet236			100884
#define SPR_pet237			100885
#define SPR_pet238			100886

#define SPR_pet245			100887
#define SPR_pet246			100888
#define SPR_pet247			100889
#define SPR_pet248			100890

#define SPR_pet256			100891
#define SPR_pet257			100892
#define SPR_pet258			100893

#define SPR_pet265			100894
#define SPR_pet266			100895
#define SPR_pet267			100896
#define SPR_pet268			100897

#define SPR_pet275			100898
#define SPR_pet276			100899
#define SPR_pet277			100900
#define SPR_pet278			100901

#define SPR_pet295			100902
#define SPR_pet296			100903
#define SPR_pet297			100904
#define SPR_pet298			100905

#define SPR_pet305			100906
#define SPR_pet306			100907
#define SPR_pet307			100908
#define SPR_pet308			100909

#define SPR_pet316			100910
#define SPR_pet317			100911
#define SPR_pet318			100912

#define SPR_pet326			100913
#define SPR_pet327			100914
#define SPR_pet328			100915

#define SPR_pet335			100916
#define SPR_pet336			100917
#define SPR_pet337			100918
#define SPR_pet338			100919

#define SPR_ride000			101000
#define SPR_ride001			101001
#define SPR_ride002			101002
#define SPR_ride003			101003
#define SPR_ride004			101004
#define SPR_ride005			101005
#define SPR_ride006			101006
#define SPR_ride007			101007
#define SPR_ride008			101008
#define SPR_ride009			101009

#define SPR_ride010			101010
#define SPR_ride011			101011
#define SPR_ride012			101012
#define SPR_ride013			101013
#define SPR_ride014			101014
#define SPR_ride015			101015
#define SPR_ride016			101016
#define SPR_ride017			101017
#define SPR_ride018			101018
#define SPR_ride019			101019

#define SPR_ride020			101020
#define SPR_ride021			101021
#define SPR_ride022			101022
#define SPR_ride023			101023
#define SPR_ride024			101024
#define SPR_ride025			101025
#define SPR_ride026			101026
#define SPR_ride027			101027
#define SPR_ride028			101028
#define SPR_ride029			101029

#define SPR_ride030			101030
#define SPR_ride031			101031
#define SPR_ride032			101032
#define SPR_ride033			101033
#define SPR_ride034			101034
#define SPR_ride035			101035
#define SPR_ride036			101036
#define SPR_ride037			101037
#define SPR_ride038			101038
#define SPR_ride039			101039

#define SPR_ride040			101040
#define SPR_ride041			101041
#define SPR_ride042			101042
#define SPR_ride043			101043
#define SPR_ride044			101044
#define SPR_ride045			101045
#define SPR_ride046			101046
#define SPR_ride047			101047
#define SPR_ride048			101048
#define SPR_ride049			101049

#define SPR_ride050			101050
#define SPR_ride051			101051
#define SPR_ride052			101052
#define SPR_ride053			101053
#define SPR_ride054			101054
#define SPR_ride055			101055
#define SPR_ride056			101056
#define SPR_ride057			101057
#define SPR_ride058			101058
#define SPR_ride059			101059

#define SPR_ride060			101060
#define SPR_ride061			101061
#define SPR_ride062			101062
#define SPR_ride063			101063
#define SPR_ride064			101064
#define SPR_ride065			101065
#define SPR_ride066			101066
#define SPR_ride067			101067
#define SPR_ride068			101068
#define SPR_ride069			101069

#define SPR_ride070			101070
#define SPR_ride071			101071
#define SPR_ride072			101072
#define SPR_ride073			101073
#define SPR_ride074			101074
#define SPR_ride075			101075
#define SPR_ride076			101076
#define SPR_ride077			101077
#define SPR_ride078			101078
#define SPR_ride079			101079

#define SPR_ride080			101080
#define SPR_ride081			101081
#define SPR_ride082			101082
#define SPR_ride083			101083
#define SPR_ride084			101084
#define SPR_ride085			101085
#define SPR_ride086			101086
#define SPR_ride087			101087
#define SPR_ride088			101088
#define SPR_ride089			101089

#define SPR_ride090			101090
#define SPR_ride091			101091
#define SPR_ride092			101092
#define SPR_ride093			101093
#define SPR_ride094			101094
#define SPR_ride095			101095
#define SPR_ride096			101096
#define SPR_ride097			101097
#define SPR_ride098			101098
#define SPR_ride099			101099

#define SPR_ride100			101100
#define SPR_ride101			101101
#define SPR_ride102			101102
#define SPR_ride103			101103
#define SPR_ride104			101104
#define SPR_ride105			101105
#define SPR_ride106			101106
#define SPR_ride107			101107
#define SPR_ride108			101108
#define SPR_ride109			101109

#define SPR_ride110			101110
#define SPR_ride111			101111
#define SPR_ride112			101112
#define SPR_ride113			101113
#define SPR_ride114			101114
#define SPR_ride115			101115
#define SPR_ride116			101116
#define SPR_ride117			101117
#define SPR_ride118			101118
#define SPR_ride119			101119

#define SPR_pet340			101152		//������
#define SPR_pet341			101153		//������
#define SPR_pet342			101154		//������
#define SPR_pet343			101155		//������
#define SPR_180m			101156		//����

#define SPR_170em			101157
#define SPR_170ax			101158
#define SPR_170cl			101159
#define SPR_170sp			101160
#define SPR_170bw			101161

#define SPR_171em			101162
#define SPR_171ax			101163
#define SPR_171cl			101164
#define SPR_171sp			101165
#define SPR_171bw			101166

#define SPR_172em			101167
#define SPR_172ax			101168
#define SPR_172cl			101169
#define SPR_172sp			101170
#define SPR_172bw			101171

#define SPR_173em			101172
#define SPR_173ax			101173
#define SPR_173cl			101174
#define SPR_173sp			101175
#define SPR_173bw			101176

#define SPR_GP_1			101177
#define SPR_GP_2			101178
#define SPR_GP_3			101179

#define SPR_DRAGON	    	101180

#define SPR_pet1115       102079 // ��ѩ��
#define SPR_pet1116       102080 // ��ѩ��

//#define SPR_pet1170       102079 // ����
//#define SPR_pet1171       102080 // �����

#define SPR_pet1180       102081 // ������
#define SPR_pet1181       102082
#define SPR_pet1182       102083
#define SPR_pet1183       102084


#define SPR_ride262		103262	// �����
#define SPR_ride263		103263
#define SPR_ride264		103264
#define SPR_ride265		103265
#define SPR_ride266		103266
#define SPR_ride267		103267
#define SPR_ride268		103268
#define SPR_ride269		103269
#define SPR_ride270		103270
#define SPR_ride271		103271
#define SPR_ride272		103272
#define SPR_ride273		103273

#define SPR_ride274		103274	// �������
#define SPR_ride275		103275
#define SPR_ride276		103276
#define SPR_ride277		103277
#define SPR_ride278		103278
#define SPR_ride279		103279
#define SPR_ride280		103280
#define SPR_ride281		103281
#define SPR_ride282		103282
#define SPR_ride283		103283
#define SPR_ride284		103284
#define SPR_ride285		103285




#ifdef _WATERANIMATION //Syu ADD ��֮��������
#define SPR_learthbubble1		101502
#define SPR_learthbubble2		101503
#define SPR_learthbubble3		101504
#define SPR_rearthbubble1		101505
#define SPR_rearthbubble2		101506
#define SPR_rearthbubble3		101507
#define SPR_charbubble1		    101508
#define SPR_charbubble2 		101509
#define SPR_whirlpool1			101510
#define SPR_whirlpool2			101511
#define SPR_goldfish1			101512
#define SPR_goldfish2			101513
#define SPR_tropicalfish1		101514
#define SPR_tropicalfish2		101515
#define SPR_turtle1				101516
#define SPR_turtle2 			101517
#define SPR_turtle3				101518
#define SPR_turtle4				101519
#define SPR_turtle5				101520
#define SPR_turtle6				101521
#define SPR_turtle7				101522
#define SPR_turtle8				101523
#define SPR_learthbubble4		101524
#define SPR_learthbubble5		101525
#define SPR_learthbubble6		101526
#define SPR_rearthbubble4		101527
#define SPR_rearthbubble5		101528
#define SPR_rearthbubble6		101529
#define SPR_whirlpool3			101579
#define SPR_lightget			101580
#define SPR_lightbreak			101581
#define SPR_lfish1				101582
#define SPR_lfish2				101583
#define SPR_lfish3				101584
#define SPR_lfish4				101585
#define SPR_lbenthos1			101586
#define SPR_lbenthos2			101587
#define SPR_rfish1				101588
#define SPR_rfish2				101589
#define SPR_rfish3				101590
#define SPR_rfish4				101591
#define SPR_rbenthos1			101592
#define SPR_rbenthos2			101593
#define SPR_bseaweed1			101594
#define SPR_bseaweed2			101595
#define SPR_bseaweed3			101596
#define SPR_bseaweed4			101597
#define SPR_mseaweed1			101598
#define SPR_mseaweed2			101599
#define SPR_mseaweed3			101600
#define SPR_mseaweed4			101601
#define SPR_sseaweed1			101602
#define SPR_sseaweed2			101603
#define SPR_sseaweed3			101604
#define SPR_sseaweed4			101605
#define SPR_smoke				101606
#endif

/*
#define SPR_pet025		100653
#define SPR_pet035		100654
#define SPR_pet036		100655
#define SPR_pet045		100656
#define SPR_pet055		100657
#define SPR_pet075		100658
#define SPR_pet096		100659
#define SPR_pet105		100660
#define SPR_pet115		100661
#define SPR_pet145		100662
#define SPR_pet185		100663
#define SPR_pet195		100664
#define SPR_pet196		100665
#define SPR_pet215		100666
#define SPR_pet225		100667
#define SPR_pet235		100668
#define SPR_pet245		100669
#define SPR_pet265		100670
#define SPR_pet275		100671
#define SPR_pet295		100672
#define SPR_pet305		100673
#define SPR_pet335		100674
*/
/*
// Fix
#define SPR_051emfix			100700		//	��g?		����
#define SPR_051axfix			100701		// 			?
#define SPR_051clfix			100702		//			??�S
#define SPR_051spfix			100703		//			?
#define SPR_051bwfix			100704		//			?

#define SPR_052emfix			100705
#define SPR_052axfix			100706
#define SPR_052clfix			100707
#define SPR_052spfix			100708
#define SPR_052bwfix			100709

#define SPR_053emfix			100710
#define SPR_053axfix			100711
#define SPR_053clfix			100712
#define SPR_053spfix			100713
#define SPR_053bwfix			100714

#define SPR_054emfix			100715
#define SPR_054axfix			100716
#define SPR_054clfix			100717
#define SPR_054spfix			100718
#define SPR_054bwfix			100719
*/
#ifdef _ITEM_CRACKER//����
#define SPR_cracker			    101401
#endif
#ifdef _SKILL_SELFEXPLODE //�Ա�
#define SPR_selfexplod			101418//101126
#endif
// ???????????????? ****************************************************/
#define CG_INVISIBLE				99	// ??????��?��?????
#define CG_MOUSE_CURSOR		25000
#define CG_GRID_CURSOR		25001

// ????????????
#define CG_BTL_BUTTON_ATTACK_UP		25100
#define CG_BTL_BUTTON_ATTACK_DOWN	25101
#define CG_BTL_BUTTON_JUJUTU_UP		25102
#define CG_BTL_BUTTON_JUJUTU_DOWN	25103
#define CG_BTL_BUTTON_CAPTURE_UP	25104
#define CG_BTL_BUTTON_CAPTURE_DOWN	25105
#define CG_BTL_BUTTON_HELP_UP		25106
#define CG_BTL_BUTTON_HELP_DOWN		25107
#define CG_BTL_BUTTON_GUARD_UP		25108
#define CG_BTL_BUTTON_GUARD_DOWN	25109
#define CG_BTL_BUTTON_ITEM_UP		25110
#define CG_BTL_BUTTON_ITEM_DOWN		25111
#define CG_BTL_BUTTON_PET_UP		25112
#define CG_BTL_BUTTON_PET_DOWN		25113
#define CG_BTL_BUTTON_ESCAPE_UP		25114
#define CG_BTL_BUTTON_ESCAPE_DOWN	25115
#define CG_BTL_BUTTON_BASE			25116	// ????�U��
#define CG_BTL_BUTTON_CROSS			25117	// ????

// �N��????
#define CG_ATR_ICON_EARTH_BIG		25120	// ?��?��
#define CG_ATR_ICON_EARTH_MDL		25121	// ?��??
#define CG_ATR_ICON_EARTH_SML		25122	// ?��?��
#define CG_ATR_ICON_WATER_BIG		25123	// ???��
#define CG_ATR_ICON_WATER_MDL		25124	// ????
#define CG_ATR_ICON_WATER_SML		25125	// ???��
#define CG_ATR_ICON_FIRE_BIG		25126	// ???��
#define CG_ATR_ICON_FIRE_MDL		25127	// ????
#define CG_ATR_ICON_FIRE_SML		25128	// ???��
#define CG_ATR_ICON_WIND_BIG		25129	// ???��
#define CG_ATR_ICON_WIND_MDL		25130	// ????
#define CG_ATR_ICON_WIND_SML		25131	// ???��

// �P�f?�N��????
#define CG_ATR_ICON_EARTH_BATTLE	25132	// ?��?
#define CG_ATR_ICON_WATER_BATTLE	25133	// ???
#define CG_ATR_ICON_FIRE_BATTLE		25134	// ???
#define CG_ATR_ICON_WIND_BATTLE		25135	// ???

// ??????????
#define CG_PET_BTL_BUTTON_BASE		25140	// ????�U��
#define CG_PET_BTL_BUTTON_WAZA_UP	25141	// ????��
#define CG_PET_BTL_BUTTON_WAZA_DOWN	25142	// ?????
#define CG_PET_BTL_BUTTON_CANCEL_UP	25143	// ????????��

// ??????
#define CG_HIT_MARK_00				25500	// �Ѣ�
#define CG_HIT_MARK_01				25501	// ����

#define CG_HIT_MARK_10				25502	// ����
#define CG_HIT_MARK_11				25503
#define CG_HIT_MARK_12				25504

#define CG_HIT_MARK_20				25505	// ����
#define CG_HIT_MARK_21				25506
#define CG_HIT_MARK_22				25507

#define CG_HIT_MARK_30				25508	// ?��
#define CG_HIT_MARK_31				25509
#define CG_HIT_MARK_32				25510

#define CG_HIT_MARK_40				25511	// ?��
#define CG_HIT_MARK_41				25512
#define CG_HIT_MARK_42				25513

// ��?
#define CG_SPEECH_BTL_OK			25520
#define CG_SPEECH_CHANGE			25521
#define CG_SPEECH_GROUP				25522
#define CG_SPEECH_SUCCESS			25523
#define CG_SPEECH_YATTA				25524
#define CG_SPEECH_HELP				25525

// ?????????
#define CG_VS_MARK_1A				25610
#define CG_VS_MARK_1B				25611
#define CG_VS_MARK_2A				25612
#define CG_VS_MARK_2B				25613
#define CG_VS_MARK_3A				25614
#define CG_VS_MARK_3B				25615
#define CG_VS_MARK_4A				25616
#define CG_VS_MARK_4B				25617
#define CG_VS_MARK_5A				25618
#define CG_VS_MARK_5B				25619
#define CG_VS_MARK_6A				25620
#define CG_VS_MARK_6B				25621
#define CG_VS_MARK_7A				25622
#define CG_VS_MARK_7B				25623
#define CG_VS_MARK_8A				25624
#define CG_VS_MARK_8B				25625
#define CG_VS_MARK_9A				25626
#define CG_VS_MARK_9B				25627
#define CG_VS_MARK_10A				25628
#define CG_VS_MARK_10B				25629


// ??
#define CG_ARROW_00					25630
#define CG_ARROW_01					25631
#define CG_ARROW_02					25632
#define CG_ARROW_03					25633
#define CG_ARROW_04					25634
#define CG_ARROW_05					25635
#define CG_ARROW_06					25636
#define CG_ARROW_07					25637
#define CG_ARROW_08					25638
#define CG_ARROW_09					25639
#define CG_ARROW_10					25640
#define CG_ARROW_11					25641
#define CG_ARROW_12					25642
#define CG_ARROW_13					25643
#define CG_ARROW_14					25644
#define CG_ARROW_15					25645

// ?????????
#define CG_CNT_DOWN_0				25900   //ս��ʱ��
#define CG_CNT_DOWN_1				25901
#define CG_CNT_DOWN_2				25902
#define CG_CNT_DOWN_3				25903
#define CG_CNT_DOWN_4				25904
#define CG_CNT_DOWN_5				25905
#define CG_CNT_DOWN_6				25906
#define CG_CNT_DOWN_7				25907
#define CG_CNT_DOWN_8				25908
#define CG_CNT_DOWN_9   			25909

// ?????????
#define CG_WND_TITLE_SYSTEM	26010
#define CG_WND_TITLE_LOGOUT	26011
#define CG_WND_TITLE_CHAT	26015
#define CG_WND_TITLE_BGM	26016
#define CG_WND_TITLE_SE		26017
#define CG_WND_TITLE_RESULT	26018
#define CG_WND_TITLE_MOUSE	26030

// ?????
#define CG_TASK_BAR_BACK		26012

// �P�f?????????
#define CG_BATTLE_BAR_PLAYER		26013	// ?????
#define CG_BATTLE_BAR_PLAYER_2		26019	// ?????
#define CG_BATTLE_BAR_PET			26014	// �\?��
#define CG_BATTLE_BAR_PET_2			26020	// ?????????

//#define CG_BTL_PET_CHANGE_WND	126230
#define CG_BTL_PET_RETURN_BTN	26041	// �P�f???????????�[??

// �[????
#define CG_CLOSE_BTN			26042	// ��??????�[??
#define CG_RETURN_BTN			26043	// ??????�[??
#define CG_OK_BTN				26093	// �ϣ�????�[??
#define CG_CANCEL_BTN			26050	// ?????????�[??

#define CG_YES_BTN				26094	// "??"???
#define CG_NO_BTN				26095	// "???"???
#define CG_EXIT_BTN				26096	// "��?"???
#define CG_SEAL_BTN				26097	// "??"???
#define CG_BUY_BTN				26098	// "??"???


#define CG_PET_WND_WAZA_BTN		26046	// ?????�[??
//#define CG_PREV_BTN				26047	// �q?��?�v??????�[??
//#define CG_NEXT_BTN				26048	// ??��?�v??????�[??

//#define CG_NAME_CHANGE_BTN		26051	// ���q��????
#define CG_NAME_CHANGE_BTN		26058	// ���q��????��
#define CG_NAME_CHANGE_BTN_DOWN	26059	// ���q��?????

#define CG_PET_WND_REST_BTN		26052	// ????????
#define CG_PET_WND_STANDBY_BTN	26053	// ???�r????
#define CG_PET_WND_BTL_BTN		26054	// ?????????
#define CG_PET_WND_MAIL_BTN		26055	// ?????????
#define CG_PET_WND_FAMILY_BTN	26245	// 
#define CG_PET_WND_STATUS_BTN	26056	// ???��????
#define CG_PET_WND_RIDE_BTN		26247	// 


// ?????????
#define CG_ITEM_WND_0		26060	// ?????????��
#define CG_ITEM_WND_GOLD_DROP_BTN_UP	26062	// ????????��
#define CG_ITEM_WND_GOLD_DROP_BTN_DOWN	26063	// ?????????
#define CG_ITEM_WND_GOLD_INC_BTN_UP		26064	// ????????��
#define CG_ITEM_WND_GOLD_INC_BTN_DOWN	26065	// ?????????
#define CG_ITEM_WND_GOLD_DEC_BTN_UP		26066	// ????????��
#define CG_ITEM_WND_GOLD_DEC_BTN_DOWN	26067	// ?????????
#define CG_ITEM_WND_JUJUTU_BTN			26069	// ??????????����???
#define CG_BTL_ITEM_WND_TITLE			26072	// �P�f???????????????

// ??????????
#define CG_STATUS_WND_VICTORY_MARK		26074	// ?????
#define CG_STATUS_WND_LV_UP_POINT		26075	// ??????��?
#define CG_STATUS_WND_UP_BTN_UP			26076	// ???????????��
#define CG_STATUS_WND_UP_BTN_DOWN		26077	// ????????????
#define CG_STATUS_WND_SHOUGOU_BTN_UP	26078	// ��????��
#define CG_STATUS_WND_SHOUGOU_BTN_DOWN	26079	// ��?????
#define CG_STATUS_WND_GROUP_BTN			26080	// ?????????

#ifdef _DELBORNPLACE //Syu ADD 6.0 ͳһ���������ִ�
#define CG_NEWBORNPET_WND				26326
#endif

#define CG_MAIL_WND_ITEM_BTN			26202	// ???????

//#define CG_MAIL_WND_MAIL_BTN			26083	// ??????
#define CG_MAIL_WND_ON_LINE_SUN_BTN		26084	// ?????????????
#define CG_MAIL_WND_ON_LINE_MOON_BTN	26088	// ??????????????
#define CG_MAIL_WND_OFF_LINE_BTN		26085	// ?????????
#define CG_MAIL_WND_MAIL_BTN			26086	// ?????
#define CG_MAIL_WND_DELETE_BTN			26087	// ?????
#define CG_MAIL_WND_CLEAR_BTN_UP		26172	// ����??????��
#define CG_MAIL_WND_CLEAR_BTN_DOWN		26173	// ����???????
#define CG_SEND_BTN						26099	// "?��"???��
#define CG_SEND_BTN_DOWN				26174	// "?��"????
//sa3 ??????????????
#define CG_MAIL_WND_ON_LINE_MERCURY_BTN	25910	// ???????????
#define CG_MAIL_WND_ON_LINE_VINUS_BTN	25911	// ???????????
#define CG_MAIL_WND_ON_LINE_EARTH_BTN	25912	// ?????????��?
#define CG_MAIL_WND_ON_LINE_MARS_BTN	25913	// ?

#define CG_ALBUM_WND_NEW_ICON			26231	// ��?????
#define CG_ALBUM_WND_SNAP_BTN_UP		26170	// ����?��?��
#define CG_ALBUM_WND_SNAP_BTN_DOWN		26171	// ����?��??

// ��?�V?�[??????
#define CG_COMMON_YES_BTN				26091	// ?????
#define CG_COMMON_NO_BTN				26092	// ??????

#ifdef __PHONEMESSAGE
#define CG_FIELD_MENU_LEFT				26100
#else
// ??????��????
#define CG_FIELD_MENU_LEFT_OLD			26100	// ?�Ƣ�?
//#ifdef __FAMILY_UI_
#define CG_FIELD_MENU_LEFT_NEW			26452	// ?�Ƣ�?
//#else
#define CG_FIELD_MENU_LEFT				26233	// ?�Ƣ�?
//#endif
#endif

#ifdef _ANGEL_SUMMON
#define CG_FIELD_ANGEL_LAMP				35270
#define CG_FIELD_MENU_LEFT_75			35271
#endif

#define CG_FIELD_MENU_BTN_OFF			26101	// ?????????�֢t
#define CG_FIELD_MENU_BTN_ON			26102	// ?????????�֢t
#define CG_FIELD_CARD_BTN_OFF			26103	// ����?????�֢t
#define CG_FIELD_CARD_BTN_ON			26104	// ����?????�֢t
#define CG_FIELD_GROUP_BTN_OFF			26105	// ?????????�֢t
#define CG_FIELD_GROUP_BTN_ON			26106	// ?????????�֢t
#define CG_FIELD_MAIL_LAMP				26107	// ???����???
#define CG_FIELD_MENU_RIGHT				26260
#define CG_FIELD_HELP_PRELAYER			26250
#define CG_FIELD_HELP_PREPAGE			26252
#define CG_FIELD_HELP_NEXTPAGE			26254
#define CG_FIELD_HELP_EXIT				26256
#define CG_FIELD_JOIN_BTL_BTN_OFF		26111	// �ӡP?????�֢t
#define CG_FIELD_JOIN_BTL_BTN_ON		26112	// �ӡP?????�֢t
#define CG_FIELD_DUEL_BTN_OFF			26113	// �n�P?????�֢t
#define CG_FIELD_DUEL_BTN_ON			26114	// �n�P?????�֢t
#define CG_FIELD_HELP_BTN_OFF			26249
#define CG_FIELD_HELP_BTN_ON			26248
#define CG_FIELD_ACT_BTN_OFF			26115	// ??????????�֢t
#define CG_FIELD_ACT_BTN_ON				26116	// ??????????�֢t
#define CG_FIELD_AM_PM_00				26117	// ��????????
#define CG_FIELD_AM_PM_01				26118	// ��????????
#define CG_FIELD_AM_PM_02				26119	// ��????????
#define CG_FIELD_AM_PM_03				26120	// ��????????
#define CG_FIELD_MENU_RIGHT_BACK		26121	// ?�Ƣ�?????
#define CG_FIELD_TRADE_BTN_OFF			26234	// TRADE?????�֢t
#define CG_FIELD_TRADE_BTN_ON			26235	// TRADE?????�֢t
#define CG_FIELD_CHANNEL_BTN_OFF		26237	// CHANNEL?????�֢t
#define CG_FIELD_CHANNEL_BTN_ON			26238	// CHANNEL?????�֢t
#define CG_FIELD_MSG_BTN_ON             26294   // Msg-Down button,added by LeiBoy
#define CG_FIELD_MSG_BTN_OFF            26295   // Msg-Up button  ,added by LeiBoy


#define CG_PET_WAZA_BAR_1	26131
#define CG_PET_WAZA_BAR_2	26132
#define CG_PET_WAZA_BAR_3	26133
#define CG_PET_WAZA_BAR_4	26134
#define CG_PET_WAZA_BAR_5	26135
#define CG_PET_WAZA_BAR_6	26136
#define CG_PET_WAZA_BAR_7	26137

// ????????
#define CG_TASK_BAR_MAP_UP		26150
#define CG_TASK_BAR_MAP_DOWM	26151
#define CG_TASK_BAR_STATUS_UP	26152
#define CG_TASK_BAR_STATUS_DOWN	26153
#define CG_TASK_BAR_PET_UP		26154
#define CG_TASK_BAR_PET_DOWN	26155
#define CG_TASK_BAR_ITEM_UP		26156
#define CG_TASK_BAR_ITEM_DOWN	26157
#define CG_TASK_BAR_MAIL_UP		26158
#define CG_TASK_BAR_MAIL_DOWN	26159
#define CG_TASK_BAR_ALBUM_UP	26160
#define CG_TASK_BAR_ALBUM_DOWN	26161
#define CG_TASK_BAR_SYSTEM_UP	26162
#define CG_TASK_BAR_SYSTEM_DOWN	26163

// �q????
#define CG_PREV_BTN				26180	// ????��
#define CG_PREV_BTN_DOWN		26181	// ?????
#define CG_NEXT_BTN				26182	// ????��
#define CG_NEXT_BTN_DOWN		26183	// ?????

#define CG_PREV_BTN2			26184	// ????��
#define CG_PREV_BTN2_DOWN		26185	// ?????
#define CG_NEXT_BTN2			26186	// ????��
#define CG_NEXT_BTN2_DOWN		26187	// ?????

#define CG_UP_BTN				26188	// ????��
#define CG_UP_BTN_DOWN			26189	// ?????
#define CG_DOWN_BTN				26190	// ????��
#define CG_DOWN_BTN_DOWN		26191	// ?????


// ????
#define CG_ICON_FUKIDASI		26500	// ????�V??????��?
#define CG_ICON_COME_ON			26501	// ?Come On!?
#define CG_ICON_GO				26502	// ?Go!?
#define CG_ICON_WATCHING		26503	// ?Watching?
#define CG_ICON_MISS			26504	// ?Miss...?
#define CG_ICON_FAIL			26505	// ?Fail...?
#define CG_ICON_GET				26506	// ?Get!!?
#define CG_ICON_COUNTER			26507	// ?Counter!?
#define CG_ICON_DANGER			26508	// ?Danger!?
#define CG_ICON_NO				26509	// ?No!?
#define CG_ICON_GUARD			26510	// ?Guard!?
#define CG_ICON_ESCAPE  		26511	// ?Escape!?
#define CG_ICON_CAPTURE 		26512	// ?Capture!?
#define CG_ICON_LEAVE			26513	// ?Leave!?
#define CG_ICON_GUARD_BREAK 	26514	// ?Guard Break!?
#define CG_ICON_CRUSH		 	26515	// ?Crush!?
#define CG_ICON_CAPTURE_UP	 	26516	// ?Capture Up!?
#ifdef _SKILL_ROAR  //�輼:���(������)
#define CG_ICON_ROAR		 	25869	// ?Hwaoo!?
#endif

// ?????��???????
#define CG_NOW_PAINTING			28999

// ????
// shan remark
//#define CG_LOGO				29000
//#define CG_LOGO				29026
// Syu ReEdit
#define CG_LOGO				35262//28995
#ifdef _v4_0
#define CG_TITLE			29025
#endif
#ifdef _NEWLOGO				   // Syu ADD 6.0���뻭����logo Ҫ��_v4_0
#define CG_TITLE			29025
#endif
#define CG_TITLE_NAME_S		29002
#define CG_TITLE_NAME_T		29003
#define CG_TITLE_NAME_O 	29004
#define CG_TITLE_NAME_N		29005
#define CG_TITLE_NAME_E		29006
#define CG_TITLE_NAME_A		29007
#define CG_TITLE_NAME_G		29008
#define CG_TITLE_NAME_E2	29009
#define CG_TITLE_NAME		29010
#define CG_TITLE_NAME_FLASH		29011
#define CG_TITLE_NAME_FLASH1	29012
#define CG_TITLE_NAME_FLASH2	29013
#define CG_TITLE_NAME_FLASH3	29014
#define CG_TITLE_NAME_FLASH4	29015
#define CG_TITLE_NAME_FLASH5	29016
#define CG_TITLE_JSS_LOGO		29017
#define CG_TITLE_DREAM_LOGO		29018
#define CG_TITLE_NOW_LOADING	29019

#ifdef _v4_0
#define CG_TITLE_ID_PASS		29046       // 29029->29046
#define CG_TITLE_ID_PASS_OK		29027
#define CG_TITLE_ID_PASS_QUIT	29028
#endif

#ifdef _NEWLOGO				   // Syu ADD 6.0���뻭����logo Ҫ��_v4_0

#ifdef _NEWPANEL_71
	#ifdef _BACK_LOGO
		#define CG_TITLE_ID_PASS        35265
	#else
		#ifdef _75_LOGIN
			#define CG_TITLE_ID_PASS		OLD_GRAPHICS_START+822
		#else
			#define CG_TITLE_ID_PASS		28997
		#endif //_75_LOGIN
	#endif //_BACK_LOGO
#else
	#ifdef _NEWPANEL_70
		#define CG_TITLE_ID_PASS		28996
	#else
		#define CG_TITLE_ID_PASS		28994       // 29046->28993
	#endif //_NEWPANEL_70
#endif //_NEWPANEL_71
#ifdef _80_LOGIN
	#undef CG_TITLE_ID_PASS
	#define CG_TITLE_ID_PASS	26301
#endif
#ifdef _BACK_LOGO
	#define CG_TITLE_ID_PASS_OK		35266
	#define CG_TITLE_ID_PASS_QUIT	35267
#else
	#ifdef _75_LOGIN
		#define CG_TITLE_ID_PASS_OK		OLD_GRAPHICS_START+823
		#define CG_TITLE_ID_PASS_QUIT	OLD_GRAPHICS_START+824
	#else
		#define CG_TITLE_ID_PASS_OK		29027
		#define CG_TITLE_ID_PASS_QUIT	29028
	#endif //_75_LOGIN
#endif //_BACK_LOGO
#endif //_NEWLOGO
#ifdef _80_LOGIN
	#undef CG_TITLE_ID_PASS_OK
	#undef CG_TITLE_ID_PASS_QUIT
	#define CG_TITLE_ID_PASS_OK			26302
	#define CG_TITLE_ID_PASS_QUIT		26303
#endif
#define CG_TITLE_SPRITEKING		29023

// ????��
#define CG_CHR_MAKE_SEL_BG		29030		// ��????�k��?��
#define CG_CHR_MAKE_BG			29031		// ��?????????��?��??��
#define CG_CHR_SEL_BG			29032		// ?????�k��?��
#define CG_CHR_SEL_LOGIN_BTN	29033		// ???????
#define CG_CHR_SEL_NEW_BTN		29034		// ��????
#define CG_CHR_SEL_DEL_BTN		29035		// ?????
#define CG_CHR_SEL_BACK_BTN		29036		// ?????
#define CG_CHR_MAKE_OK_BTN		29037		// ��???? ?�e???
#define CG_CHR_MAKE_BACK_BTN	29038		// ��???? ?????
#define CG_CHR_MAKE_SEL2_BG		29039		// �����ԡk��?��
#define CG_CHR_MAKE_HOME_NAME0	29040		// ��������??????�Z?
#define CG_CHR_MAKE_HOME_NAME1	29041		// ��������??????�Z?
#define CG_CHR_MAKE_HOME_NAME2	29042		// ��������??????�Z?
#define CG_CHR_MAKE_HOME_NAME3	29043		// ��������???????�Z?
#define CG_CHR_MAKE_EYE_SEL		29044		// ��???? ???�k��
#define CG_CHR_MAKE_NOSE_SEL	29045		// ��???? ��??�k��
#define CG_CHR_MAKE_FACE		30000		// ???????��

//	trade sys
//#define CG_TRADE_WND			40001
#define	CG_TRADE_LEFT_BTN_UP	26180
#define	CG_TRADE_LEFT_BTN_DOWN	26181
#define	CG_TRADE_RIGHT_BTN_UP	26182
#define	CG_TRADE_RIGHT_BTN_DOWN	26183
#define	CG_TRADE_UP_BTN_UP		26064
#define	CG_TRADE_UP_BTN_DOWN	26065
#define	CG_TRADE_DOWN_BTN_UP	26066
#define	CG_TRADE_DOWN_BTN_DOWN	26067
#define	CG_TRADE_PUT_BTN_UP		26062
#define	CG_TRADE_PUT_BTN_DOWN	26063
#define	CG_TRADE_OK_BTN			26093
#define	CG_TRADE_CANCEL_BTN		26050
#define	CG_TRADE_LOCK_BTN		26300


// family 
//#define CG_FAMILY_ADD_WIN		26139
//#define CG_FAMILY_CHANNEL_BTN_OFF		26234
//#define CG_FAMILY_CHANNEL_BTN_ON		26235

#define CG_EXIT_BTN2				26241
#define CG_EXIT_BTN2_DOWN		26242
#define CG_JOIN_BTN				26243
#define CG_JOIN_BTN_DOWN		26244

#define CG_TRADE_MARK			26246

#ifdef _ANGEL_SUMMON
#define CG_ANGEL_MARK			26246
#endif

//andy_ride
#define RIDE_OLD	0x00
// ride Pet table
typedef struct
{
	int rideNo;
	int charNo;
	int petNo;
	//ANDY_ADD
	int rideCode;
} tagRidePetTable;

static tagRidePetTable ridePetTable[] =
{
	{ 101000, 100000, 100352, RIDE_OLD },
	{ 101001, 100005, 100352, RIDE_OLD },
	{ 101002, 100010, 100352, RIDE_OLD },
	{ 101003, 100015, 100352, RIDE_OLD },
	{ 101004, 100000, 100329, RIDE_OLD },
	{ 101005, 100005, 100327, RIDE_OLD },
	{ 101006, 100010, 100330, RIDE_OLD },
	{ 101007, 100015, 100328, RIDE_OLD },
	{ 101008, 100700, 100372, RIDE_OLD },
	{ 101009, 100705, 100373, RIDE_OLD },
#ifdef _8_RIDEPETS
	{ 101989, 100705, 101576, RIDE_OLD },
	{ 101978, 100705, 101532, RIDE_OLD },
#endif
	{ 101010, 100020, 100396, RIDE_OLD },
	{ 101011, 100025, 100396, RIDE_OLD },
	{ 101012, 100030, 100396, RIDE_OLD },
	{ 101013, 100035, 100396, RIDE_OLD },
	{ 101014, 100020, 100328, RIDE_OLD },
	{ 101015, 100025, 100329, RIDE_OLD },
	{ 101016, 100030, 100327, RIDE_OLD },
	{ 101017, 100035, 100330, RIDE_OLD },
	{ 101018, 100710, 100372, RIDE_OLD },
	{ 101019, 100715, 100373, RIDE_OLD },
#ifdef _8_RIDEPETS
	{ 101988, 100715, 101576, RIDE_OLD },
	{ 101986, 100715, 101532, RIDE_OLD },
#endif
	{ 101020, 100040, 100351, RIDE_OLD },
	{ 101021, 100045, 100351, RIDE_OLD },
	{ 101022, 100050, 100351, RIDE_OLD },
	{ 101023, 100055, 100351, RIDE_OLD },
	{ 101024, 100040, 100328, RIDE_OLD },
	{ 101025, 100045, 100330, RIDE_OLD },
	{ 101026, 100050, 100327, RIDE_OLD },
	{ 101027, 100055, 100329, RIDE_OLD },
	{ 101028, 100720, 100372, RIDE_OLD },
	{ 101029, 100725, 100373, RIDE_OLD },
#ifdef _8_RIDEPETS
	{ 101965, 100725, 101576, RIDE_OLD },
	{ 101975, 100725, 101532, RIDE_OLD },
#endif
	{ 101030, 100060, 100353, RIDE_OLD },
	{ 101031, 100065, 100353, RIDE_OLD },
	{ 101032, 100070, 100353, RIDE_OLD },
	{ 101033, 100075, 100353, RIDE_OLD },
	{ 101034, 100060, 100329, RIDE_OLD },
	{ 101035, 100065, 100327, RIDE_OLD },
	{ 101036, 100070, 100330, RIDE_OLD },
	{ 101037, 100075, 100328, RIDE_OLD },
	{ 101038, 100730, 100372, RIDE_OLD },
	{ 101039, 100735, 100373, RIDE_OLD },
#ifdef _8_RIDEPETS
	{ 101966, 100735, 101576, RIDE_OLD },
	{ 101976, 100735, 101532, RIDE_OLD },
#endif
	{ 101040, 100080, 100396, RIDE_OLD },
	{ 101041, 100085, 100396, RIDE_OLD },
	{ 101042, 100090, 100396, RIDE_OLD },
	{ 101043, 100095, 100396, RIDE_OLD },
	{ 101044, 100080, 100328, RIDE_OLD },
	{ 101045, 100085, 100327, RIDE_OLD },
	{ 101046, 100090, 100330, RIDE_OLD },
	{ 101047, 100095, 100329, RIDE_OLD },
	{ 101048, 100740, 100372, RIDE_OLD },
	{ 101049, 100745, 100373, RIDE_OLD },
#ifdef _8_RIDEPETS
	{ 101973, 100745, 101576, RIDE_OLD },
	{ 101984, 100745, 101532, RIDE_OLD },
#endif
	{ 101050, 100100, 100353, RIDE_OLD },
	{ 101051, 100105, 100353, RIDE_OLD },
	{ 101052, 100110, 100353, RIDE_OLD },
	{ 101053, 100115, 100353, RIDE_OLD },
	{ 101054, 100100, 100329, RIDE_OLD },
	{ 101055, 100105, 100328, RIDE_OLD },
	{ 101056, 100110, 100330, RIDE_OLD },
	{ 101057, 100115, 100327, RIDE_OLD },
	{ 101058, 100750, 100372, RIDE_OLD },
	{ 101059, 100755, 100373, RIDE_OLD },
#ifdef _8_RIDEPETS
	{ 101974, 100755, 101576, RIDE_OLD },
	{ 101985, 100755, 101532, RIDE_OLD },
#endif
	{ 101060, 100120, 100354, RIDE_OLD },
	{ 101061, 100125, 100354, RIDE_OLD },
	{ 101062, 100130, 100354, RIDE_OLD },
	{ 101063, 100135, 100354, RIDE_OLD },
	{ 101064, 100120, 100327, RIDE_OLD },
	{ 101065, 100125, 100330, RIDE_OLD },
	{ 101066, 100130, 100328, RIDE_OLD },
	{ 101067, 100135, 100329, RIDE_OLD },
	{ 101068, 100760, 100372, RIDE_OLD },
	{ 101069, 100765, 100373, RIDE_OLD },
#ifdef _8_RIDEPETS
	{ 101967, 100765, 101576, RIDE_OLD },
	{ 101977, 100765, 101532, RIDE_OLD },
#endif
	{ 101070, 100140, 100354, RIDE_OLD },
	{ 101071, 100145, 100354, RIDE_OLD },
	{ 101072, 100150, 100354, RIDE_OLD },
	{ 101073, 100155, 100354, RIDE_OLD },
	{ 101074, 100140, 100327, RIDE_OLD },
	{ 101075, 100145, 100329, RIDE_OLD },
	{ 101076, 100150, 100330, RIDE_OLD },
	{ 101077, 100155, 100328, RIDE_OLD },
	{ 101078, 100770, 100372, RIDE_OLD },
	{ 101079, 100775, 100373, RIDE_OLD },
#ifdef _8_RIDEPETS
	{ 101968, 100775, 101576, RIDE_OLD },
	{ 101979, 100775, 101532, RIDE_OLD },
#endif
	{ 101080, 100160, 100352, RIDE_OLD },
	{ 101081, 100165, 100352, RIDE_OLD },
	{ 101082, 100170, 100352, RIDE_OLD },
	{ 101083, 100175, 100352, RIDE_OLD },
	{ 101084, 100160, 100330, RIDE_OLD },
	{ 101085, 100165, 100329, RIDE_OLD },
	{ 101086, 100170, 100327, RIDE_OLD },
	{ 101087, 100175, 100328, RIDE_OLD },
	{ 101088, 100780, 100372, RIDE_OLD },
	{ 101089, 100785, 100373, RIDE_OLD },
#ifdef _8_RIDEPETS
	{ 101970, 100785, 101576, RIDE_OLD },
	{ 101981, 100785, 101532, RIDE_OLD },
#endif
	{ 101090, 100180, 100351, RIDE_OLD },
	{ 101091, 100185, 100351, RIDE_OLD },
	{ 101092, 100190, 100351, RIDE_OLD },
	{ 101093, 100195, 100351, RIDE_OLD },
	{ 101094, 100180, 100328, RIDE_OLD },
	{ 101095, 100185, 100330, RIDE_OLD },
	{ 101096, 100190, 100329, RIDE_OLD },
	{ 101097, 100195, 100327, RIDE_OLD },
	{ 101098, 100790, 100372, RIDE_OLD },
	{ 101099, 100795, 100373, RIDE_OLD },
#ifdef _8_RIDEPETS
	{ 101969, 100795, 101576, RIDE_OLD },
	{ 101980, 100795, 101532, RIDE_OLD },
#endif
	{ 101100, 100200, 100353, RIDE_OLD },
	{ 101101, 100205, 100353, RIDE_OLD },
	{ 101102, 100210, 100353, RIDE_OLD },
	{ 101103, 100215, 100353, RIDE_OLD },
	{ 101104, 100200, 100329, RIDE_OLD },
	{ 101105, 100205, 100328, RIDE_OLD },
	{ 101106, 100210, 100327, RIDE_OLD },
	{ 101107, 100215, 100330, RIDE_OLD },
	{ 101108, 100800, 100372, RIDE_OLD },
	{ 101109, 100805, 100373, RIDE_OLD },
#ifdef _8_RIDEPETS
	{ 101972, 100805, 101576, RIDE_OLD },
	{ 101983, 100805, 101532, RIDE_OLD },
#endif
	{ 101110, 100220, 100396, RIDE_OLD },
	{ 101111, 100225, 100396, RIDE_OLD },
	{ 101112, 100230, 100396, RIDE_OLD },
	{ 101113, 100235, 100396, RIDE_OLD },
	{ 101114, 100220, 100330, RIDE_OLD },
	{ 101115, 100225, 100327, RIDE_OLD },
	{ 101116, 100230, 100329, RIDE_OLD },
	{ 101117, 100235, 100328, RIDE_OLD },
	{ 101118, 100810, 100372, RIDE_OLD },
	{ 101119, 100815, 100373, RIDE_OLD },
#ifdef _8_RIDEPETS
	{ 101971, 100815, 101576, RIDE_OLD },
	{ 101982, 100815, 101532, RIDE_OLD },
#endif
	{ 101179, 101177, 101178, RIDE_OLD},
	{ 101179, 101178, 101177, RIDE_OLD},
};

#define RIDE_PET0	1
#define RIDE_PET1	( 1 << 1 )
#define RIDE_PET2	( 1 << 2 )
#define RIDE_PET3	( 1 << 3 )
#define RIDE_PET4	( 1 << 4 )
#define RIDE_PET5	( 1 << 5 )
#define RIDE_PET6	( 1 << 6 )
#define RIDE_PET7	( 1 << 7 )
#define RIDE_PET8	( 1 << 8 )
#define RIDE_PET9	( 1 << 9 )
#ifdef _8_RIDEPETS
#define RIDE_PET10	( 1 << 10 )
#define RIDE_PET11	( 1 << 11 )
#define RIDE_PET12	( 1 << 12 )
#define RIDE_PET13	( 1 << 13 )
#define MAXNOINDEX 32*4
#else
#define MAXNOINDEX 10
#endif

//andy_add 2002/6/24
typedef struct	{
	int petNo;
	int learnCode;
}tagRideCodeMode;


static tagRideCodeMode RideCodeMode[] = {
	{ 100374, RIDE_PET0 },
	{ 100358, RIDE_PET1 },
	{ 100362, RIDE_PET2 },
	{ 100279, RIDE_PET3 },
	{ 100288, RIDE_PET4 },
	{ 100283, RIDE_PET5 },
	{ 100346, RIDE_PET6 },
	{ 100310, RIDE_PET7 },
	{ 100372, RIDE_PET8 },
	{ 100373, RIDE_PET9 }
#ifdef _8_RIDEPETS
	,{ 101576, RIDE_PET10 }
	,{ 101532, RIDE_PET11 }

	,{ 100873, RIDE_PET12 }
	,{ 100907, RIDE_PET13 }
#endif
//#ifdef RIDE_PET_LIMIT
//	,{ 100372, RIDE_PET_BOOK}
//	,{ 100373, RIDE_PET_BOOK}
//#endif
};

//andy_add 2002/6/24
typedef struct	{
	int RideNo[MAXNOINDEX];
}tagRideNoList;

static tagRideNoList RideNoList[] = {
#ifdef _8_RIDEPETS
	{ 101305, 101306, 101307, 101308, 101309, 101310, 101311, 101312, 101008, 101009, 101978, 101989,100425,100420,100445,0},	//–°∞´◊”
	{ 101313, 101314, 101315, 101316, 101317, 101318, 101319, 101320, 101018, 101019, 101986, 101988,100425,100420,100445,0},	//»¸—«»À
	{ 101321, 101322, 101323, 101324, 101325, 101326, 101327, 101328, 101028, 101029, 101975, 101965,100425,100420,100445,0},	//±Ë◊”ƒ–∫¢
	{ 101329, 101330, 101331, 101332, 101333, 101334, 101335, 101336, 101038, 101039, 101976, 101966,100425,100420,100445,0},	//ø·∏Á
	{ 101337, 101338, 101339, 101340, 101341, 101342, 101343, 101344, 101048, 101049, 101984, 101973,100425,100420,100445,0},	//–‹∆§ƒ–
	{ 101345, 101346, 101347, 101348, 101349, 101350, 101351, 101352, 101058, 101059, 101985, 101974,100425,100420,100445,0},	//¥Û∏ˆ
	{ 101353, 101354, 101355, 101356, 101357, 101358, 101359, 101360, 101068, 101069, 101977, 101967,100425,100420,100445,0},	//–°∞´√√
	{ 101361, 101362, 101363, 101364, 101365, 101366, 101367, 101368, 101078, 101079, 101979, 101968,100425,100420,100445,0},	//–‹∆§√√
	{ 101369, 101370, 101371, 101372, 101373, 101374, 101375, 101376, 101088, 101089, 101981, 101970,100425,100420,100445,0},	//√±◊”√√
	{ 101377, 101378, 101379, 101380, 101381, 101382, 101383, 101384, 101098, 101099, 101980, 101969,100425,100420,100445,0},	//∂Ã∑®∑¢º–√√
	{ 101385, 101386, 101387, 101388, 101389, 101390, 101391, 101392, 101108, 101109, 101983, 101972,100425,100420,100445,0},	// ÷Ã◊≈Æ
	{ 101393, 101394, 101395, 101396, 101397, 101398, 101399, 101400, 101118, 101119, 101982, 101971,100425,100420,100445,0},	//¿±√√

#else
	{ 101305, 101306, 101307, 101308, 101309, 101310, 101311, 101312, 101008, 101009},
	{ 101313, 101314, 101315, 101316, 101317, 101318, 101319, 101320, 101018, 101019},
	{ 101321, 101322, 101323, 101324, 101325, 101326, 101327, 101328, 101028, 101029},
	{ 101329, 101330, 101331, 101332, 101333, 101334, 101335, 101336, 101038, 101039},
	{ 101337, 101338, 101339, 101340, 101341, 101342, 101343, 101344, 101048, 101049},
	{ 101345, 101346, 101347, 101348, 101349, 101350, 101351, 101352, 101058, 101059},
	{ 101353, 101354, 101355, 101356, 101357, 101358, 101359, 101360, 101068, 101069},
	{ 101361, 101362, 101363, 101364, 101365, 101366, 101367, 101368, 101078, 101079},
	{ 101369, 101370, 101371, 101372, 101373, 101374, 101375, 101376, 101088, 101089},
	{ 101377, 101378, 101379, 101380, 101381, 101382, 101383, 101384, 101098, 101099},
	{ 101385, 101386, 101387, 101388, 101389, 101390, 101391, 101392, 101108, 101109},
	{ 101393, 101394, 101395, 101396, 101397, 101398, 101399, 101400, 101118, 101119},
#endif
};

typedef struct	{
	int charNo;
	int Noindex;
}tagRidePetList;

static tagRidePetList RPlistMode[] = {
	{ 100000, 0 }, { 100005, 0 }, { 100010, 0 }, { 100015, 0 },	//С����
	{ 100020, 1 }, { 100025, 1 }, { 100030, 1 }, { 100035, 1 },	//������
	{ 100040, 2 }, { 100045, 2 }, { 100050, 2 }, { 100055, 2 },	//�����к�
	{ 100060, 3 }, { 100065, 3 }, { 100070, 3 }, { 100075, 3 },	//���
	{ 100080, 4 }, { 100085, 4 }, { 100090, 4 }, { 100095, 4 },	//��Ƥ��
	{ 100100, 5 }, { 100105, 5 }, { 100110, 5 }, { 100115, 5 },	//���

	{ 100120, 6 }, { 100125, 6 }, { 100130, 6 }, { 100135, 6 },	//С����
	{ 100140, 7 }, { 100145, 7 }, { 100150, 7 }, { 100155, 7 },	//��Ƥ��
	{ 100160, 8 }, { 100165, 8 }, { 100170, 8 }, { 100175, 8 },	//ñ����
	{ 100180, 9 }, { 100185, 9 }, { 100190, 9 }, { 100195, 9 },	//�̷�������
	{ 100200, 10 }, { 100205, 10 }, { 100210, 10 }, { 100215, 10 },	//����Ů
	{ 100220, 11 }, { 100225, 11 }, { 100230, 11 }, { 100235, 11 },	//����
};

#endif

