# -*- coding: utf-8 -*-
import mod.client.extraClientApi as clientApi
import localplayermodule

ClientSystem = clientApi.GetClientSystemCls()


class SelfAttackSystem(ClientSystem):
    def __init__(self, namespace, systemName):
        super(SelfAttackSystem, self).__init__(namespace, systemName)
        self.ListenForEvent(
            clientApi.GetEngineNamespace(),
            clientApi.GetEngineSystemName(),
            'OnScriptTickClient', self, self.on_tick)

    def on_tick(self):
        localplayermodule.local_player_attack_entity(clientApi.GetLocalPlayerId())


_inst = SelfAttackSystem(clientApi.GetEngineNamespace(), "SelfAttackSystem")
