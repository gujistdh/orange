# -*- coding: utf-8 -*-
import mod.client.extraClientApi as clientApi
import time
import threading

ClientSystem = clientApi.GetClientSystemCls()

class MountClientSystem(ClientSystem):
    def __init__(self, namespace, systemName):
        super(MountClientSystem, self).__init__(namespace, systemName)
        self.ListenForEvent(
            clientApi.GetEngineNamespace(),
            clientApi.GetEngineSystemName(),
            "ClickChatSendClientEvent",
            self,
            self.on_chat
        )

    def _summon(self):
        pid = clientApi.GetLocalPlayerId()
        factory = clientApi.GetEngineCompFactory()
        try:
            act = factory.CreateAction(pid)
            if act:
                act.Dismount()
        except:
            pass
        try:
            pet_sys = clientApi.GetSystem('Minecraft', 'pet')
            if pet_sys:
                mgr = pet_sys.GetMountMgrClient()
                if mgr:
                    mgr.RequestSummonMount()
                    def mount():
                        time.sleep(0.5)
                        try:
                            pid2 = clientApi.GetLocalPlayerId()
                            game = clientApi.GetEngineCompFactory().CreateGame(clientApi.GetLevelId())
                            ents = game.GetEntitiesAround(pid2, 6.0)
                            act2 = clientApi.GetEngineCompFactory().CreateAction(pid2)
                            for eid in ents:
                                if eid != pid2:
                                    try:
                                        act2.Mount(eid)
                                        break
                                    except:
                                        continue
                        except:
                            pass
                    t = threading.Thread(target=mount)
                    t.setDaemon(True)
                    t.start()
        except:
            pass

    def on_chat(self, args):
        msg = args.get('message', '').strip()
        if msg != '/坐骑':
            return
        args['cancel'] = True
        args['message'] = ''
        self._summon()


try:
    if '_mount_inst' in globals() and globals()['_mount_inst'] is not None:
        globals()['_mount_inst'].Destroy()
except:
    pass
_mount_inst = MountClientSystem(clientApi.GetEngineNamespace(), "MountSystem")