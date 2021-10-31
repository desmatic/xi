<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class cando {

    const GUEST = 1;            /* this permission is always allowed */
    const LOGIN = 2;            /* user account (anyone who has been authenticated) */
    const AGENT = 3;            /* agents */
    const SUPERAGENT = 4;       /* agent team leader */
    const SUPERVISOR = 5;       /* agent supervisors */
    const MANAGER = 6;          /* supervisor managers */
    const ACCOUNTS = 7;         /* payment and financial information */
    const DIRECTOR = 8;         /* company director */
    const SUPPORT = 9;          /* IT support */
    const DEVELOPER = 10;       /* software developers, access to everything */
    const VIEW_DEBUG = 11;
    const ADD_ARTICLE_CONTROLLER_ROOT = 12;
    const PIN_ARTICLE_CONTROLLER_ROOT = 13;
    const EDIT_ARTICLE_CONTROLLER_ROOT = 14;
    
}