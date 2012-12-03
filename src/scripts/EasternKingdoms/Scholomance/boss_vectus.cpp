/*
 * Copyright (C) 2010-2012 OregonCore <http://www.oregoncore.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2012 ScriptDev2 <http://www.scriptdev2.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Boss_Vectus
SD%Complete: 100
SDComment:
SDCategory: Scholomance
EndScriptData */

#include "ScriptPCH.h"

enum eEnums
{
    EMOTE_GENERIC_FRENZY_KILL   = -1000001,

    SPELL_FLAMESTRIKE            = 18399,
    SPELL_BLAST_WAVE             = 16046,
    SPELL_FIRESHIELD             = 19626,
    SPELL_FRENZY                 = 8269 //28371,
};

struct boss_vectusAI : public ScriptedAI
{
    boss_vectusAI(Creature *c) : ScriptedAI(c) {}

    uint32 m_uiFireShield_Timer;
    uint32 m_uiBlastWave_Timer;
    uint32 m_uiFrenzy_Timer;

    void Reset()
    {
        m_uiFireShield_Timer = 2000;
        m_uiBlastWave_Timer = 14000;
        m_uiFrenzy_Timer = 0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;

        //FireShield_Timer
        if (m_uiFireShield_Timer <= uiDiff)
        {
            DoCast(me, SPELL_FIRESHIELD);
            m_uiFireShield_Timer = 90000;
        }
        else
            m_uiFireShield_Timer -= uiDiff;

        //BlastWave_Timer
        if (m_uiBlastWave_Timer <= uiDiff)
        {
            DoCast(me->getVictim(), SPELL_BLAST_WAVE);
            m_uiBlastWave_Timer = 12000;
        }
        else
            m_uiBlastWave_Timer -= uiDiff;

        //Frenzy_Timer
        if (me->GetHealth()*100 / me->GetMaxHealth() < 25)
        {
            if (m_uiFrenzy_Timer <= uiDiff)
            {
                DoCast(me, SPELL_FRENZY);
                DoScriptText(EMOTE_GENERIC_FRENZY_KILL, me);

                m_uiFrenzy_Timer = 24000;
            }
            else
                m_uiFrenzy_Timer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_vectus(Creature* pCreature)
{
    return new boss_vectusAI (pCreature);
}

void AddSC_boss_vectus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_vectus";
    newscript->GetAI = &GetAI_boss_vectus;
    newscript->RegisterSelf();
}
