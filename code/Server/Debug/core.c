#include ".setup"
#include "../Hardware/Memory/.setup"
#include<linux/string.h>

static LIST_HEAD(nodes);

Fn(void,Server,Debug,Output,Write)(StructMemory(Server,Debug,Output)*sdo,u8*data,u32 len)
{
	if(!sdo||!sdo->file)
		return;
	loff_t pos=sdo->file->f_pos;
	kernel_write(sdo->file,data,len,&pos);
	sdo->file->f_pos=pos;
}

Fn(void,Server,Debug,Output,Message,Write)(StructMemory(Server,Debug,Output,Message)*sdom)
{
	if(!sdom||!sdom->sdo)
		return;
	u32 len=0;
	while(sdom->message[len])
		len++;
	Call(Server,Debug,Output,Write)(sdom->sdo,sdom->message,len);
}

Fn(StructMemory(Server,Debug,Output)*,Server,Debug,Output,New)(u8*path)
{
	u8*copy=kstrdup((char*)path,GFP_KERNEL);
	if(!copy)
		return NULL;
	for(u32 i=0;copy[i];i++)
		if(copy[i]=='_'||copy[i]==','||copy[i]==' ')
			copy[i]='/';
	struct file*file=filp_open((char*)copy,O_WRONLY|O_CREAT|O_TRUNC,0644);
	kfree(copy);
	if(IS_ERR(file))
		return NULL;
	MemoryGet(sdo,Server,Debug,Output)
	{
		filp_close(file,NULL);
		return NULL;
	}
	sdo->file=file;
	INIT_LIST_HEAD(&sdo->sdom_node);
	list_add_tail(&sdo->node,&nodes);
	return sdo;
}

Fn(StructMemory(Server,Debug,Output,Message)*,Server,Debug,Output,Message,New)(StructMemory(Server,Debug,Output)*sdo,u8*message)
{
	if(!sdo||!message)
		return NULL;
	StructMemory(Server,Debug,Output,Message)*sdom;
	list_for_each_entry(sdom,&sdo->sdom_node,node)
	{
		if(sdom->message&&strcmp((char*)sdom->message,(char*)message)==0)
			return sdom;
	}
	MemoryGet(sdomn,Server,Debug,Output,Message)return NULL;
	sdomn->sdo=sdo;
	sdomn->message=kstrdup((char*)message,GFP_KERNEL);
	if(!sdomn->message)
	{
		MemoryDelete(sdomn);
		return NULL;
	}
	INIT_LIST_HEAD(&sdomn->sdomv_node);
	list_add_tail(&sdomn->node,&sdo->sdom_node);
	return sdomn;
}

Fn(void,Server,Debug,Output,Message,Value,New)(StructMemory(Server,Debug,Output,Message)*sdom,u8*value)
{
	if(!sdom||!value)
		return;
	StructMemory(Server,Debug,Output,Message,Value)*sdomv;
	list_for_each_entry(sdomv,&sdom->sdomv_node,node)
	{
		if(sdomv->value&&strcmp((char*)sdomv->value,(char*)value)==0)
			return;
	}
	MemoryGet(sdomvn,Server,Debug,Output,Message,Value)return;
	sdomvn->value=kstrdup((char*)value,GFP_KERNEL);
	if(!sdomvn->value)
	{
		MemoryDelete(sdomvn);
		return;
	}
	list_add_tail(&sdomvn->node,&sdom->sdomv_node);
	
	Call(Server,Debug,Output,Message,Write)(sdom);
	Call(Server,Debug,Output,Write)(sdom->sdo,(u8*)": ",2);
	u32 vlen=0;
	while(sdomvn->value[vlen])
		vlen++;
	Call(Server,Debug,Output,Write)(sdom->sdo,sdomvn->value,vlen);
	Call(Server,Debug,Output,Write)(sdom->sdo,(u8*)"\n",1);
}

Fn(void,Server,Debug,Output,Delete)(StructMemory(Server,Debug,Output)*sdo)
{
	if(!sdo)
		return;
	StructMemory(Server,Debug,Output,Message)*sdom,*stmp;
	list_for_each_entry_safe(sdom,stmp,&sdo->sdom_node,node)
	{
		StructMemory(Server,Debug,Output,Message,Value)*sdomv,*vtmp;
		list_for_each_entry_safe(sdomv,vtmp,&sdom->sdomv_node,node)
		{
			list_del(&sdomv->node);
			kfree(sdomv->value);
			MemoryDelete(sdomv);
		}
		list_del(&sdom->node);
		kfree(sdom->message);
		MemoryDelete(sdom);
	}
	if(sdo->file)
		filp_close(sdo->file,NULL);
	MemoryDelete(sdo);
}

InitLibrary(Server,Debug)
{
	MemoryPrepare(Server,Debug,Output);
	MemoryPrepare(Server,Debug,Output,Message);
	MemoryPrepare(Server,Debug,Output,Message,Value);
}

QuitLibrary(Server,Debug)
{
}
