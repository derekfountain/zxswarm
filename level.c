#include <stdint.h>
#include <arch/zx.h>
#include "level.h"
#include "levels.h"
#include "main.h"

uint8_t  current_num_virions;
uint8_t  current_frame;
uint16_t frames_before_change;
uint16_t immune_frames;


void init_level( LEVEL *level )
{
  zx_border( level->border_colour );

  current_num_virions  = level->starting_num_virions;
  current_frame        = 0;
  frames_before_change = 0;
  immune_frames        = 0;
 
  (level->draw_frame)();

  return;
}

void apply_virion_logic( LEVEL *level, VIRION *v )
{
  if( ! v->active )
    return;

  if( v->x_i < 0 || v->x_i > 255
      ||
      v->y_i < 0 || v->y_i > 191 )
    return;

  uint8_t x = v->x_i;
  uint8_t y = v->y_i;

  uint8_t attribute = *(zx_pxy2aaddr(x,y));

  if( attribute == PAPER_RED )
  {
    v->active = 0;
    current_num_virions--;
  }
  else if( attribute == PAPER_GREEN )
  {
    if( current_num_virions < level->max_virions )
    {
      extern VIRION swarm[MAX_IN_SWARM];
      
      uint8_t i;
      for( i=0; i<MAX_IN_SWARM; i++ )
      {
	if( ! swarm[i].active )
	{
	  swarm[i].active = 1;
	  current_num_virions++;
	  break;
	}
      }
    }
  }
  
}


uint8_t get_current_num_virions(void)
{
  return current_num_virions;
}
