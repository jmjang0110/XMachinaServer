#pragma once


/// +-----------------------------------------------
///					 PacketSendBuf 
/// ________________________________________________
/// 
/// �� ���ǻ���
///  	
/// - WSASend()�� �� �� �Ϸ� ������ �ö����� �����Ͱ� 
///   �����Ǿ���Ѵ�. ==>  std::shared_ptr �� ��������!
/// -----------------------------------------------+

/*
	������ ��Ŷ�� �������� ���̵� ���� ��Ŷ�� ũ��� �������ִ�.
	ex) Transform ��Ŷ �̶�� �� �� sizeof(float) * 3 * 3 �̰���?

	�׷��� �޸� Ǯ�� ����� ����. 
	�� ��Ŷ�� �ش��ϴ� �޸� Ǯ�� �̸� ������ ����� ���� 
	��Ŷ�� ���� �� �޸� Ǯ���� �޸𸮸� ������ 
	Make_Shared �� ���� ������ ���� ��ٰ�
	�ٽ� �޸� Ǯ�� ���ٰ� ����.

	�� ��Ŷ�� �´� �޸𸮸� �޸� Ǯ���� �ܾ� �ö��� 
	unordered_map �� �̿�����
	O(1) 

	�׷��ٸ� ���� ���� ���۴� ��� �ϳ�?
	�׷��� 32 64 128 256 512 ����Ʈ �޸�Ǯ���� �̸� ��������
	����ٰ� ���� !
*/


class PacketSendBuf
{
private:
	BYTE*					mBuffer          = {}; // �޸� ������
	UINT32					mBufferTotalSize = 0;  // SListMemoryPool ���� ������ �޸��� ��ü ũ�� 
	UINT32					mWriteSize       = 0;  // �޸𸮿��� ������ ũ�� 

public:
	PacketSendBuf(BYTE* buffer, UINT32 allocSize);
	~PacketSendBuf();

	BYTE*	GetBuffer()	   { return mBuffer; }
	UINT32	GetTotalSize() { return mBufferTotalSize; }
	UINT32	GetWriteSize() { return mWriteSize; }
	void	Close(UINT32 writeSize); 

};

