/** 
 * Ȩ�޿���apply����
 * ���أ�Ϊ���������أ�Ϊ�ܾ�
 */
STATIC_VAR_TAG int DEBUG = 0;

// controls the use of the bind() efun
int valid_bind(object binder, object old_owner, object new_owner)
{
    object ob;

    if(DEBUG){
        write("[MASTER_OB]->valid_bind():" + binder + "\n");
        write("([old : " + old_owner + ", new : " + new_owner + "])\n");
    }

    if (ob = find_object(SECURITY_D))
        return (int)ob->valid_bind(binder, old_owner, new_owner);

    return 1;
}

// Each of the database efunctions calls valid_database() prior to executing.
mixed valid_database(object caller, string func, mixed *info)
{
    if(DEBUG > 1){
        write("[MASTER_OB]->valid_database():\n");
        write("([caller : " + caller + ", func : " + func + "])\n");
    }
    if (func == "connect")
        return "secret"; // mysql���ݿ�����
    else
        return 1;
}

// valid_hide: controls the use of the set_hide() efun, to hide objects or
// see hidden objects
int valid_hide(object who)
{
    if(DEBUG)
        write("[MASTER_OB]->valid_hide():" + who + "\n");
    return 1;
}

// valid_link: controls use of the link() efun for creating hard links
// between paths
int valid_link(string original, string reference)
{
    if(DEBUG)
        write("[MASTER_OB]->valid_link():" + original + "-" + reference + "\n");
    return 0;
}

// valid_object: controls whether an object loaded by the driver should exist
int valid_object(object ob)
{
    if(DEBUG)
        write("[MASTER_OB]->valid_object():" + ob + "\n");
    return (! clonep(ob)) || inherits(F_MOVE, ob);
}

// valid_override: controls which simul_efuns may be overridden with
// efun:: prefix and which may not.  This function is only called at
// object compile-time
int valid_override(string file, string name, string main_file)
{
    if(DEBUG)
        write("[MASTER_OB]->valid_override():" + file + "(" + name + ")\n");

    // simul_efun can override any simul_efun by Annihilator
    if (file == SIMUL_EFUN_OB || file == MASTER_OB)
        return 1;

    // Must use the move() defined in F_MOVE.
    if ((name == "move_object") && file != F_MOVE && file != F_COMMAND)
        return 0;

    if ((name == "destruct") && ! sscanf(file, "/adm/single/simul_efun/%s", file))
        return 0;

    //  may also wish to protect destruct, shutdown, snoop, and exec.
    return 1;
}

// valid_read: read privileges; called exactly the same as valid_write()
int valid_read(string file, mixed user, string func)
{
    object ob;
    if (DEBUG)
        write("[MASTER_OB]->valid_read():" + file + "(" + func + ")\n");

    if (ob = find_object(SECURITY_D))
        return (int)ob->valid_read(file, user, func);

    return 1;
}

// valid_save_binary: controls whether an object can save a binary
//   image of itself to the specified "save binaries directory"
//   (see config file)
int valid_save_binary(string filename)
{
    if(DEBUG)
        write("[MASTER_OB]->valid_save_binary():" + filename + "\n");
    return 1;
}

// valid_seteuid: determines whether an object ob can become euid str
int valid_seteuid(object ob, string str)
{
    if(DEBUG){
        write("[MASTER_OB]->valid_seteuid():\n");
        write("([obj : " + ob + ", euid : " + str + "])\n");
    }
    if (find_object(SECURITY_D))
        return (int)SECURITY_D->valid_seteuid( ob, str );
    return 1;
}

// valid_shadow: controls whether an object may be shadowed or not
int valid_shadow(object ob)
{
    object pre;

    if(DEBUG)
        write("[MASTER_OB]->valid_shadow():" + ob + "\n");

    pre = previous_object();
    if (geteuid(pre) == ROOT_UID ||
        sscanf(file_name(pre), "/shadow/%*s"))
    {
        return 1;
    }

    log_file("shadow", sprintf("%O try to shadow %O failed.\n", pre, ob));
    return 0;
}

// valid_socket: controls access to socket efunctions
int valid_socket(object eff_user, string fun, mixed *info)
{
    if(DEBUG)
        write("[MASTER_OB]->valid_socket():" + eff_user + "(" + fun + ")\n");
    return 1;
}

// valid_write: write privileges; called with the file name, the object
//   initiating the call, and the function by which they called it.
int valid_write(string file, mixed user, string func)
{
    object ob;
    if(DEBUG)
        write("[MASTER_OB]->valid_write():" + file + "(" + func + ")\n");
    if (ob = find_object(SECURITY_D))
        return (int)ob->valid_write(file, user, func);

    return 0;
}

/**
 * ����apply��ʧЧ
 */
 
// valid_asm: controls whether or not an LPC->C compiled object can use asm {}
int valid_asm(string file)
{
    if(DEBUG)
        write("[MASTER_OB]->valid_asm():" + file + "\n");
    return 1;
}

// valid_compile: controls whether or not an file can be compiled
int valid_compile(string file)
{
    if(DEBUG)
        write("[MASTER_OB]->valid_compile():" + file + "\n");
    if (! find_object(VERSION_D))
        return 1;

    if (VERSION_D->is_release_server())
        return 1;

    return file_valid(file) == 1;
}