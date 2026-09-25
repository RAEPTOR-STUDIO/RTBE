#pragma once
#include "RaeptorCogs/Domain/PannelCog/dock.h"
#include <RaeptorCogs/Cog/domain.h>

typedef struct RaeptorPannelCog_dock_space RaeptorPannelCog_dock_space_t;

DomainFunction(RaeptorPannelCog_dock_space_t *, RTBE_get_dock_space, void);
DomainFunction(void, RTBE_add_dock, RaeptorPannelCog_dock_t *dock);
DomainFunction(void, RTBE_remove_dock, RaeptorPannelCog_dock_t *dock);
