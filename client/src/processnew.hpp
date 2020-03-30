/*
 * =====================================================================================
 *
 *       Filename: processnew.hpp
 *        Created: 08/14/2015 02:47:30 PM
 *    Description:
 *
 *  (0, 0): screen coord origin
 *
 *   +---------------------------------------------------------------------------------
 *   |
 *   |  (OffX, OffY)
 *   |
 *   |       - xooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo+
 *   |       ^ 0                                                                   0
 *   |       | 0                       +------------------+                        0
 *   |       | 0                  ID : |                  |                        0
 *   |       | 0                       +------------------+                        0
 *   |       | 0                       +------------------+                        0
 *   |       | 0                 PWD : |                  |                        0
 *   |       | 0                       +------------------+                        0
 *   |       H 0                       +------------------+                        0
 *   |       | 0         CONFIRM PWD : |                  |                        0
 *   |       | 0                       +------------------+                        0
 *   |       | 0                                                                   0
 *   |       | 0                                                                   0
 *   |       | 0                 +----------+         +----------+                 0
 *   |       | 0                 |  Create  |         |   Exit   |                 0
 *   |       | 0                 +----------+         +----------+                 0
 *   |       | 0                                                                   0
 *   |       v 0                                                                   0
 *   |       - +ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo+
 *   |         |<-----------------------------W----------------------------------->|
 *   |
 *
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */
#pragma once

#include "set"
#include <cstdint>
#include <SDL2/SDL.h>

#include "idbox.hpp"
#include "process.hpp"
#include "message.hpp"
#include "textbutton.hpp"
#include "labelboard.hpp"
#include "passwordbox.hpp"
#include "tritexbutton.hpp"

class ProcessNew: public Process
{
    private:
        enum CheckState: int
        {
            CHECK_NONE    = 0,
            CHECK_OK      = 1,
            CHECK_ERROR   = 2,
            CHECK_PENDING = 3,
        };

    private:
        const int m_w;
        const int m_h;

    private:
        const int m_x;
        const int m_y;

    private:
        int m_CheckID;
        int m_CheckPwd;
        int m_CheckPwdConfirm;

    private:
        labelBoard m_LBID;
        labelBoard m_LBPwd;
        labelBoard m_LBPwdConfirm;

    private:
        IDBox       m_BoxID;
        PasswordBox m_BoxPwd;
        PasswordBox m_BoxPwdConfirm;

    private:
        labelBoard m_LBCheckID;
        labelBoard m_LBCheckPwd;
        labelBoard m_LBCheckPwdConfirm;

    private:
        TextButton m_TBCreate;
        TextButton m_TBExit;

    private:
        std::set<std::string> m_IDCache[2];

    public:
        ProcessNew();
        virtual ~ProcessNew() = default;

    public:
        int ID() const
        {
            return PROCESSID_NEW;
        }

    public:
        void Update(double);
        void Draw();
        void processEvent(const SDL_Event &);

    private:
        void DoPostAccount();
        void DoExit();

    private:
        // post account operation to server
        // server should respond with SMAccount for the post
        // operation : 0 : validate only
        //             1 : create
        //             2 : login
        void PostAccount(const char *, const char *, int);

    private:
        bool LocalCheckID(const char *);
        bool LocalCheckPwd(const char *);

    private:
        void CacheAdd(bool bValid, std::string szID)
        {
            m_IDCache[bValid ? 0 : 1].insert(szID);
        }

        void CacheRemove(bool bValid, std::string szID)
        {
            m_IDCache[bValid ? 0 : 1].erase(szID);
        }

        void CacheClear(bool bValid)
        {
            m_IDCache[bValid ? 0 : 1].clear();
        }

        bool CacheFind(bool bValid, std::string szID)
        {
            return m_IDCache[bValid ? 0 : 1].find(szID) != m_IDCache[bValid ? 0 : 1].end();
        }

    private:
        void CheckInput();
};
