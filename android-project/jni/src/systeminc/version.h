#ifndef __VERSION_H__
#define __VERSION_H__

//#include <crtdbg.h>


/* �뽫����ר���� definition �����������
 * Ȼ����� remark/unmark �����ع���
 */

//ʯ���汾����
//#define __3_0					   // ( 3.0 ��) Cary ʯ��3.0�Ŀ���
//#define _SA_VERSION_60		   // ( 6.0 ��) WON ADD ʯ��ʱ��6.0 �İ汾����
#define _SA_VERSION_70			   // ( 7.0 ��) WON ADD ʯ��ʱ��7.0 �İ汾����


#define SHOW_VERSION "2010.10.1"

//#define _DEFAULT_PKEY "Wonwonwon"		// (�ɿ���)( ʯ�� 6.0 )
//#define _RUNNING_KEY  "11111111"		// (�ɿ���)( ʯ�� 6.0 )
#define _DEFAULT_PKEY "www.longzoro.com"		// (�ɿ���)( ʯ�� 6.0 )
#define _RUNNING_KEY  "www.longzoro.com"		// (�ɿ���)( ʯ�� 6.0 )

//#define _DEFAULT_PKEY "643218509"
//#define _RUNNING_KEY  "432645177375" 



/* -------------------------------------------------------------------
 * ר��������ְ������幦�ܲ���
 * ��Ա��С褡���ѫ��־ΰ��С��
 * ˵����
 */
#define  _FMVER21                      // (�ɿ���) ����ְ�ƹ���
#define _PERSONAL_FAME                 // (�ɿ���) Arminius 8.30 �����������
#define _LEADERRIDE	 				   // (�ɿ���) �Ĵ�����ﱩ���� Robin 0921
 
// -------------------------------------------------------------------
// ר���������½��NPC��̸����
// ��Ա������
// ˵����__EDEN_EFFECT:�ĳ�������NPC'ί�н��׵�NPC
//			 __EDEN_AUCTION:ί��NPC������Ʒ����
#define __EDEN_EFFECT			// (�ɿ���) ������ʱҪ��
#define __EDEN_AUCTION		    // (�ɿ���) ������ʱҪ��

// -------------------------------------------------------------------
// ר��������������������
// ��Ա����־
// ˵����
#define __ATTACK_MAGIC

// -------------------------------------------------------------------
// ר���������޵ı���
// ��Ա������
// ˵����
#define _ITEM_FIRECREAKER

/* -------------------------------------------------------------------
 * �¹��ܣ�PK��ϵ
 * ��Ա����ѫ
 * ˵����
 */
//#define _PKSERVER // (���ɿ���)


// -------------------------------------------------------------------
// ר���������ֻ���Ѷ����ҽ���ʵ��
// ��Ա������  (LeiBoy)
// ˵��������ҿ��Դ���Ѷ���ض���ASP��ҳ����
// ��Ҫ��ص� : PhoneMsg.cpp
// ������� : 2002��01��28��
// �������� : 2002��03��04��
// Status : Ready!
//#define __PHONEMESSAGE    
//#define _FIX_URLENCODE             // WON ADD (�� �� ��) ����������Ѷ�޷��ӿո������


// -------------------------------------------------------------------
// ר���������ֻ���Ѷ�Ķ��ڹ���
// ��Ա��(ԭ��)����  (�Ӱ���δ��)
// ˵��������ҿ��Դ���Ѷ����Ƭ�к��ѵ��ֻ���
// �������� : ��ֹ����!!
// Status : ������...
//#define __CARDMESSAGE   //(����)!!


//#define __NEW_CLIENT_ONLY_WRITE	//Cary(����,δ���)
//#define CHINA_VER			//Cary (����)
//#define __ALBUM_4
//#define __ALBUM_6			//Cary �����µĳ����ಾ(������)
//#define __TEST_SERVER		//Cary 3.0�Ĳ���server
//#define __EMOTION			//Cary ����ı��飬ֻ��client
#define __HELP				//Cary ����˵��
#define __NEW_PROTOCOL		//Cary �µĵײ�
//#define __NEW_CLIENT		//Cary 
//#define __NEW_CLIENT_MEM	//Cary
#define __SKYISLAND			//Cary ������յ��Ĺ���
#define __HI_COLOR			//Cary ����߲ʵĹ���(debug versionר��)
#define __TALK_TO_NPC		//Cary ʹ�������npc˵��
#define __NEW_BATTLE_EFFECT	//Cary 2002.2.21 �µ�ս����Ч����:��������
#define _AFTER_TRADE_PETWAIT_	   // (�� �� ��) set pet status to wait after trade :vincent

/* -------------------------------------------------------------------
 * �¹��ܣ�����4
 * ��Ա��־��
 * ˵����
 */
#define _FIX_DEL_MAP			   // (�ɿ���) WON ADD ������ҳ��ͼ

#define _FIX_9_FMPOINT			   // (�ɿ���) WON ADD �Ŵ�ׯ԰
#define _FIX_10_FMPOINT			//(�ɿ�) Change ʮ��ׯ԰ 20050404
#ifdef _FIX_10_FMPOINT
#define fm_point_num	10		   // ׯ԰��
#else
#define fm_point_num	9		   // (�ɿ���) WON ADD ׯ԰��
#endif

#define _ADD_POOL_ITEM			   // (�ɿ���) WON ADD ���Ӽķŵ���
//#define _ADD_FAMILY_TAX	       // (���ɿ�) WON ADD ����ׯ԰˰��

#define _LOST_FOREST_FOG           // (�ɿ���) the lostforest's fog. code:shan 
#define _MIND_ICON                 // (�ɿ���) show icon from char mind. code:shan
#define _MAGIC_WEAKEN              // (�ɿ���) show magic(weaken). code:shan

#define __ALBUM_47				   // ������ֻ






// -------------------------------------------------------------------
// vincent define
#define _MAGIC_DEEPPOISION         // (�ɿ���) show magic(deeppoison). code:vincent
#define _MAGIC_BARRIER             // (�ɿ���) show magic(barrier). code:vincent
#define _MAGIC_NOCAST              // (�ɿ���) show magic(nocast). code:vincent
#define _ITEM_CRACKER              // (�ɿ���) ����:���� code:vincent
#define _SKILL_ROAR                // (�ɿ���) vincent  �輼:���(������)
#define _SKILL_SELFEXPLODE         // (�ɿ���) vincent  �輼:�Ա�

#define _NEW_RIDEPETS			   // (�ɿ���) �����

#define _ATTDOUBLE_ATTACK	       // (�ɿ���) ����ǿ������
#define __TOCALL_MAGIC			   // (�ɿ���) kjl 02/06/24	
#define _VARY_WOLF                 // (�ɿ���) pet skill : vary wolf. code:shan
#define _PIRATE_ANM                // (�ɿ���) monster move(������). code: shan
#define _TRADE_PETCOLOR            // (�ɿ���) Syu ADD �ı佻��ʱ����ת����ɫ
#define _FIXSHOWMPERR			   // (�ɿ���) Syu ADD ��������ʱ������ʾ����
#define _LIZARDPOSITION			   // (�ɿ���) Syu ADD ����������ʾλ�ù���


/* -------------------------------------------------------------------
 * �¹��ܣ�
 * ��������2002/12/18 14:00 ����
*/

#define _TEAM_KICKPARTY				// (�ɿ���) ANDY �ӳ�����
#define _MONEYINPUT					// (�ɿ���) Syu ADD �ֶ������Ǯ��

#define _PETS_SELECTCON				// (�ɿ���) ANDY ����ѡ��



/* -------------------------------------------------------------------
 * �¹��ܣ�ʯ��6.0
 * ��������
*/
#define _OBJSEND_C				   // (�ɿ���) ANDY 6.0

#define _ADD_SHOW_ITEMDAMAGE			   // (�ɿ���) WON ADD ��ʾ���ߵ��;ö�

#define _WATERANIMATION			   // (�ɿ���) Syu ADD ��֮��������
#define _NEWDRAWBATTLEMAP		   // (�ɿ���) Syu ADD ˮ����ս���ر���ս����Ч 
#define _AniCharBubble		       // (�ɿ���) Syu ADD �����������³�����
#define _AniCrossFrame		       // (�ɿ���) Syu ADD �������ι���������
#define _AniImmobile			   // (�ɿ���) Syu ADD ��������ض�����
#define _AniRandom				   // (�ɿ���) Syu ADD ���������������
//#define _DELBORNPLACE			   // (�ɿ���) Syu ADD 6.0 ͳһ���������ִ�
#define _NEWLOGO				   // (�ɿ���) Syu ADD 6.0���뻭����logo Ҫ��_v4_0 
#define _NEWMUSICFILE6_0		   // (�ɿ���) Syu ADD 6.0 �µ�ͼ����

#define _TRADESHOWNAME			   // (�ɿ���) Syu ADD ������ʾ�Է�����
#define _TRADEINPUTMONEY		   // (�ɿ���) Syu ADD �����ֶ������Ǯ
#define _TRADE_SHOWHP			   // (�ɿ���) Syu ADD ������ʾѪ��
#define _CHANGETRADERULE		   // (�ɿ���) Syu ADD ���׹����޶�

#define _PETSKILL_CANNEDFOOD	   // (�ɿ���) ANDY ���＼�ܹ�ͷ
#define _SPECIALSPACEANIM		   // (�ɿ���) Syu ADD ���ⳡ����������
#define _SHOOTCHESTNUT			   // (�ɿ���) Syu ADD �輼��������



/* -------------------------------------------------------------------
 * �¹��ܣ�ʯ��7.0
 * ��������
*/
//�ѿ���
#define _SARS						// (�ɿ���) WON ADD ��ɷ����
#define _FIX_MAXGOLD			    // (�ɿ���) WON ADD ���������Ǯ����

#define _ITEM_PILENUMS				// (�ɿ���) ANDY �����زĶѵ�

#define _ITEM_EQUITSPACE			// (�ɿ���) ANDY ����װ����λ
#define _EQUIT_ARRANGE				// (�ɿ���) ANDY ��
#define _EQUIT_NEWGLOVE				// (�ɿ���) ANDY ������λ

#define _LOCKPARTYFLOOR				// (�ɿ���) Syu ADD ���Ӳ�����ӵĵ�ͼ���
#define _LOCKHELP_OK				// (�ɿ���) Syu ADD �������ɼ���ս��

#define _TRADESYSTEM2				// (�ɿ���) Syu ADD �½���ϵͳ

#define _TRADETALKWND				// (�ɿ���) Syu ADD ���������Ի����
#define _TRADELOCKBTN				// (�ɿ���) Syu ADD ����������

#define _WAVETRACK					// (�ɿ���) Syu ADD ������������

#define _CHAR_NEWLOGOUT				// (�ɿ���) ANDY �µǳ�����
//end deleted by lsh

//deleted by lsh on 2011.08.03

//�¹��ܣ�����ְҵ
#define _TABLEOFSKILL				// (�ɿ���) Syu ADD 7.0 �������＼�ܱ�
//#define _NEWPANEL_71				// (�ɿ���) ROG ADD ��login����
//#define	_NEWPANEL_70				// (�ɿ���) Syu ADD ʯ��7.0 LOGIN ����
#define _CHAR_PROFESSION			// (�ɿ���) WON ADD ����ְҵ
#define _SKILLSORT					// (�ɿ���) Syu ADD ְҵ�������з���
#define _NEWREQUESTPROTOCOL			// (�ɿ���) Syu ADD ����ProtocolҪ������ϸ��
#define _OUTOFBATTLESKILL			// (�ɿ���) Syu ADD ��ս��ʱ����Protocol
#define _NEWSOUNDEFFECT				// (�ɿ���) Syu ADD ������Ч��
#define _FIXSTATUS					// (�ɿ���) Syu ADD ��ս��״̬��ʾ��ʽ
#define _ALLDOMAN					// (�ɿ���) Syu ADD ���а�NPC
#define _CHATROOMPROTOCOL			// (�ɿ���) Syu ADD ������Ƶ��
#define _FRIENDCHANNEL				// (�ɿ���) ROG ADD ����Ƶ��
#define	_TELLCHANNEL				// (�ɿ���) ROG ADD ����Ƶ��

#define _DROPPETWND					// (�ɿ���) Syu ADD ��������ȷ��

#define _MAILSHOWPLANET				// (�ɿ���) Syu ADD ��ʾ��Ƭ����
#define _MOON_FAIRYLAND				// (�ɿ���) ROG ADD ��֮�ɾ�
#define _SPECIALSPACEANIM_FIX		// (�ɿ���) ROG ADD ��������
//#define _TIMEBAR_FUNCTION			// (���ɿ�) ROG ADD ʱ��bar��ʽ


//10/23
#define _FM_MODIFY				// ���幦���޸�
//end deleted by lsh

#define _HI_COLOR_16				// (�ɿ�) ANDY (8bit to 16bit) ��ͼ��
#define _FIREHUNTER_SKILL			// (�ɿ���) ROG ADD ��ȸ����_������ɱ
#define _PETSKILL_ACUPUNCTURE       // (�ɿ���) Change �輼:�����Ƥ
#define _PETSKILL_DAMAGETOHP	    // (�ɿ���) Change �輼:���¿���(��Ѫ���ı���) 
#define _PETSKILL_BECOMEFOX         // (�ɿ���) Change �輼:�Ļ���
#define _PETSKILL_BECOMEPIG         // (�ɿ�) Change �輼:������


#define _CHANNEL_MODIFY		// Ƶ����������

// �¹��ܣ����������Ͽ�(�¸��ѻ���)
//#define _NEW_WGS_MSG				// (�ɿ�) WON ADD WGS�����Ӵ�

#define _CHANGEGALAXY // Robin ��ϵ����
#define _ERROR301 // Robin AP�ش�301����,��Ʒ��δע��
//end deleted by lsh

#define _STANDBYPET // Robin �ʹ������б�Server

#define	DIRECTINPUT_VERSION 0x0700	// Robin ����DirectX SDK��������

//#define _CLOUD_EFFECT				// (���ɿ�) ROG ADD	�Ʋ�Ч��
//#define _NPC_WELFARE_2			// (���ɿ�) WON ADD ְҵNPC-2

//#define _NPC_SELLSTH				// (�ɿ���) ANDY ����
//#define _SYUTEST
//#define _SYUTEST2
//#define _SYUTEST3
//#define _AUCPROTOCOL				// (���ɿ�) Syu ADD ����Ƶ������Protocol

/* -------------------------------------------------------------------
 * �¹��ܣ�PK�ŷ���
 * ��������
*/
//������
//������

//#define _PKSERVERCHARSEL			// (���ɿ�) Syu ADD PK�ŷ���ѡ����ϵ����

//#define _PK2007
//#define HITFLAG_FONT

//PK�������Ա����
#define PK_SERVICE_MAN
// -------------------------------------------------------------------
// ���²��ɿ�
/* -------------------------------------------------------------------
 * �¹��ܣ�real.bin��ͼ
 * ��������
*/
//������
//#define _SAHOOK				    // (���ɿ�) Syu ADD Hook��ʽ
//#define _REALDIVISION			    // (���ɿ�) Syu ADD real.bin��ͼ��Ҫ��realtxt.txt��
// -------------------------------------------------------------------
//#define _OMIT_WGS					// (���ɿ�) �Թ��ʺš�������WGS. code:shan
// -------------------------------------------------------------------
//#define _CHECK_GAMESPEED		    // (���ɿ�) ANDY ������
/* -------------------------------------------------------------------
 * �¹��ܣ�ʯ��5.0
 * ˵����
 */
//#define _CANT_ATK					// (���ɿ�) can't attack(own team). code: shan
//#define _SETTICK_COUNT			// (���ɿ�) ANDY �ظ�����
//#define _GM_MARK					// (���ɿ�) WON ADD GM ʶ��
//#define _GM_IDENTIFY		        // (���ɿ�)Rog ADD GMʶ���gmָ��

//#define _HI16_REEDITIMAGE			// (���ɿ�) ANDY (8bit to 16bit) ͼ������

//#define _LOGINKICK				// (���ɿ�) Syu ADD ���뻭�泬��30���߻���ϵ�б�
/* -------------------------------------------------------------------
 * �¹��ܣ�ʯ�� 800 * 600 
 * ��Ա�����,����
 * ˵����
*/
//#define _NEW_RESOMODE  //800 600ģʽ
//#define _NEWDRAWBATTLEMAP_FIX		   // (���ɿ�)ROG ADD ս����ͼ�޸�
/* ��Ҫ������ClientǰҪ������ֵ��Ϊ0 */
//#define DISPLACEMENT_X 384
//#define DISPLACEMENT_Y 288
//#define DISPLACEMENT_X 160
//#define DISPLACEMENT_Y 120
#define DISPLACEMENT_X 0
#define DISPLACEMENT_Y 0

//#define _DEBUG
#define SDL_SA

// Terry define start -------------------------------------------------------------------
// �����û��memory leak ----------------------------------------------------------------
#ifdef _DEBUG
//#define MALLOC(s) _malloc_dbg(s,_CLIENT_BLOCK,__FILE__,__LINE__)
//#define malloc(c,s) _calloc_dbg(c,s,_CLIENT_BLOCK,__FILE__,__LINE__)
//#define new new(1,__FILE__,__LINE__)
#define FREE(p) free(p)
//_free_dbg(p, 4)
#else
#define MALLOC(s) malloc(s)
//#define malloc(c,s) calloc(c,s)
//#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#define FREE(p) free(p)
#endif
// --------------------------------------------------------------------------------------
//ε���N���ˌaǻ�ٕ[�µ̶�
//#ifdef _DEBUG  // wxy �q��
//#define PROFILE_TEST
#define _PET_TRACE_MOUSE
//#define __EDEN_DICE  				// (���ɿ�) �ĳ�����
//#endif  //wxy �q��
//-----------------------
unsigned int TimeGetTime(void);
// Terry define end   -------------------------------------------------------------------

//���̰����
//#define _BACK_VERSION //���̰�


#ifdef _BACK_VERSION
	#define _REMAKE_20				// ʯ�����̹��ܿ���(�����)
	#define _BACK_LOGO
	#define _TIME_GET_TIME		// ��QueryPerformanceCounterȡ��timeGetTime
	#define _BACK_WINDOW //���̰��Ӵ���(debug��ſɿ�,�ڲ���Աʹ��)
	#define _READ16BITBMP			// Terry �ɶ��뼰��ʾ�߲�ͼ
#else
	#define _ATTACK_AI					//�ڹҹ���AI
	#define _AI_OTHER					// AI��������
	#define _AI_NEW_FIRST			// AI �״ο�����ְҵ����
	#define __AI				//Cary �����Զ�ս������
	#define _BATTLESKILL				// (�ɿ���) Syu ADD ս�����ܽ���
	#define _NEWPANEL				    // (�ɿ���) Syu ADD 7.0 ������״̬����
#endif

//end modified by lsh

/* -------------------------------------------------------------------
 * �¹��ܣ�ʯ��7.5
 * ��������
*/
#define _READ16BITBMP			// Terry �ɶ��뼰��ʾ�߲�ͼ
#define _HI_COLOR_32
#define _ANGEL_SUMMON			// ��ʹ���л� Robin
#define _PROFESSION_ADDSKILL  // (�ɿ�) Change ׷��ְҵ����
#define _TRANS_6				// (�ɿ�) Change ����6ת
#define _PET_2TRANS			// (�ɿ�) Change ����2ת
//end deleted by lsh

//#define _75_LOGIN					// 7.5��login���� 
#define _SURFACE_ANIM     // ��̬��������ʾ
#define _NPC_FUSION
#define _STREET_VENDOR		// ��̯����
#define _NEW_MANOR_LAW		// ��ׯ԰����

#define _PETSKILL_LER			// �׶�����
//#define _PK_SERVER            //����PK�
//#define _8_TEST					//8.0����server

#define _STREET_VENDOR_CHANGE_ICON	// �޸İ�̯ͼʾ

//#define _TALK_WINDOW				// �Ի��Ӵ�
#define _SHOW_FUSION				// (�ɿ�) Change �ںϳ�����CLIENT��ֻ��ʾת����,�޸�Ϊ�ںϳ���ʾΪ�ں�

#define _FONT_SIZE					// Robin (Ҫ��) �ɸ����ʹ�С
#define _CONTRACT					// Robin (Ҫ��) ��Լ����
#define _HALLOWEEN_EFFECT			// ��ʥ����Ч
#define _ADD_STATUS_2				// ������������״̬�ڶ���
#define _PETSKILL_BATTLE_MODEL		// ���＼��ս��ģ��
#define _RACEMAN					// cyg �Գ�����npc
#define _PETSKILL_ANTINTER			// (�ɿ�) Change �輼:��֮��
#define _PETSKILL_EXPLODE			// (�ɿ�) Change �輼:���ѹ���
#define _ITEM_FIREWORK				// �̻���
#define _FIREWORK_SOUND				// cyg ������Ч(�̻���)
//#define _GET_HOSTNAME				// (�ɿ�) Change ȡ�õ�������
#define _SKILL_ADDBARRIER			// (�ɿ�) Change �輼:Ϊħ�������쳣���Թ���
#define _PETSKILL_PEEL				// (�ɿ�) Change �輼:����ѿ�


//#define _MOUSE_DBL_CLICK			// Robin �����Ҽ�˫������

//--------------------------------------------------------------------------------------------------------
//ר�� 8.0 ʧ������� �¼ӹ��ܶ����� ��Ա����ѫ ���� ���� �ݿ�
//--------------------------------------------------------------------------------------------------------
#define _PET_ITEM				// Terry �����װ������
#define _NPC_PICTURE			// (�ɿ�) Change �¶ĳ�����(��ʱ�����NPC)
#define _JOBDAILY				// cyg ������־����
#define _ITEM_PATH				// (�ɿ�) Change ��̤�ϲ���������ĵ���
#define _ALCHEMIST				// Robin �������� 
#define _ALCHEPLUS				// Robin �������� 
#define _EXTEND_AB				// cyg ������Ƭ����
#define _ITEM_INTENSIFY			// (�ɿ�) Change �輼:ǿ������
//#define _TEACHER_SYSTEM			// Terry ��ʦϵͳ
#define _80_LOGIN				// Terry 8.0 ���뻭��
#define _8_RIDEPETS				// (�ɿ�) Change 8.0���
#define _8BATTLEMAP				// Change 8.0 ս���ر�
#define _NEW_EARTH				// Terry �µ���,�ɿ����ڲ��ŷ���
//#define _80_LOGIN_PLAY		// Terry 8.0 ���뻭��
#endif
