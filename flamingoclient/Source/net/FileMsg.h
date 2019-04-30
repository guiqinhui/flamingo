/** 
 * Э�����Ͷ���, FileMsg.h
 * zhangyl 2017.03.17
 **/
#pragma once

#include <stdint.h>

enum file_msg_type
{
    file_msg_type_unknown,
    msg_type_upload_req,
    msg_type_upload_resp,
    msg_type_download_req,
    msg_type_download_resp,
};

enum file_msg_error_code
{
    file_msg_error_unknown,        //δ֪����
    file_msg_error_progress,       //�ļ��ϴ��������ؽ�����
    file_msg_error_complete,       //�ļ��ϴ������������
    file_msg_error_not_exist       //�ļ�������
};

#pragma pack(push, 1)
//Э��ͷ
struct file_msg
{
    int64_t  packagesize;       //ָ������Ĵ�С
};

#pragma pack(pop)

/** 
 *  �ļ��ϴ�
 */
/**
    �ͻ��ˣ�cmd = msg_type_upload_req, seq, filemd5, offset ,filesize, filedata
    ������Ӧ�� cmd = msg_type_upload_resp, seq, errorcode, filemd5, offset, filesize
 **/

/** 
 *  �ļ�����
 */
/** 
    �ͻ��ˣ�cmd = msg_type_download_req, seq, filemd5, offset, filesize, filedata
    ������: cmd = msg_type_download_resp, seq, errorcode, filemd5, offset, filesize, filedata

 **/

