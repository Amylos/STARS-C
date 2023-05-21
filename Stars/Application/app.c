/**
 * @file: 	app.c
 *
 * @date: 	<date>
 * @author: <author>
 *
 */

/**
 * For testing purpose of entity star.
 * Comment this line to remove these tests.
 */
#define TEST_ISOLATE_ENTITY_STAR


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <assert.h>

#include <star.h>
#include <container.h>
#include "app.h"


#define mBitsSet(f,m)		((f)|=(m))
#define mBitsClr(f,m)		((f)&=(~(m)))
#define mBitsTgl(f,m)		((f)^=(m))
#define mBitsMsk(f,m)		((f)& (m))
#define mIsBitsSet(f,m)		(((f)&(m))==(m))
#define mIsBitsClr(f,m)		(((~(f))&(m))==(m))

/**
 * @brief The application status masks.
 */
enum s_statusMasks{
	ST_AL_CLEARED				= 0x00000000,
	ST_APP_INIT_FAILED			= 0x80000000,
	ST_APP_SDL_INITIATED		= 0x00000001,
	ST_APP_MOUSE_BUTTON_DOWN	= 0x00000002,
	ST_APP_PAUSED				= 0x00000004,
};

/**
 * @brief The application structure definition.
 */
	struct s_app{
		uint32_t		m_uStatus;			/* application status flags */
		SDL_Window	*	m_pWindow;			/* main window structure */
		SDL_Renderer*	m_pRenderer;		/* main renderer structure */
		SDL_TimerID		m_timerID;			/* animation timer ID */

		/********************************************************************************/

		SDL_Rect		m_space;			/* the space area delimiter */
		t_container	*	m_pStars;			/* the stars container */
		int				m_generateTimer;	/* remaining time before star re-generating */
		SDL_Point		m_generateOrigin;	/* re-generation area location */
		SDL_Point		m_ptStart;			/* for mouse location tracking */
		SDL_Point		m_MousePosition;

#ifdef TEST_ISOLATE_ENTITY_STAR
		/** ---------FOR TESTING PURPOSE--------------
		 * @todo Declare here two star entity pointers
		 */
		t_star* pStarOne;
		t_star*	pStarTwo;
#endif

	};

/**
 * @brief Animation callback function prototype.
 * 		  Performs all tasks for animating and drawing graphical objects.
 * 
 * @param interval number of milliseconds between two call back.
 * @param pApp pointer to the application structure.
 * @return Uint32 interval in milliseconds for the next call back.
 */
Uint32 _AppTimerCallback(Uint32 interval, t_app*pApp);


/**
 * @brief App "constructor".
 *        Performs an app entity creation by allocating memory need by
 *        the app to store its parameters. Initializes the app
 *        parameters by creating all necessary components.
 * 
 * @return t_app* the pointer to the newly created app.
 */
t_app*AppNew(void){
	t_app*pApp=(t_app*)malloc(sizeof(t_app));
	assert(pApp);

	srand((unsigned int)time(NULL));

	*pApp=(t_app){
		/**
		 * @todo Initializing the space area. 
		 */ 

		.m_space =(SDL_Rect){
			.x = APP_SPACE_PADDING,
			.y = APP_SPACE_PADDING,
			.h = APP_WINDOW_HEIGHT - APP_SPACE_PADDING*2,
			.w = APP_WINDOW_WIDTH - APP_SPACE_PADDING*2,
		},

		/**
		 * @todo Creating the star container.
		 */ 
		.m_pStars = ContainerNew((t_ptfV)StarDel),
		.m_generateTimer = 100,

		/**
		 * @brief Initializing the initial generation point
		 */
		.m_generateOrigin=(SDL_Point){
 			APP_WINDOW_WIDTH/2,
			APP_WINDOW_HEIGHT/2
 		}
	};

	if(SDL_Init(SDL_INIT_VIDEO)!=0){
		fprintf(stderr,"Failed to initiate SDL: %s\n", SDL_GetError());
		mBitsSet(pApp->m_uStatus, ST_APP_INIT_FAILED);
		return pApp;
	}
	mBitsSet(pApp->m_uStatus, ST_APP_SDL_INITIATED);

	pApp->m_pWindow=SDL_CreateWindow(
			APP_WINDOW_TITLE_STR,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			APP_WINDOW_WIDTH,
			APP_WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN);
	if(pApp->m_pWindow==NULL){
		fprintf(stderr,"Failed to create window: %s\n", SDL_GetError());
		mBitsSet(pApp->m_uStatus, ST_APP_INIT_FAILED);
		return pApp;
	}

	pApp->m_pRenderer=SDL_CreateRenderer(pApp->m_pWindow, -1, SDL_RENDERER_ACCELERATED);
	if(pApp->m_pRenderer==NULL){
		fprintf(stderr,"Failed to create accelerated renderer: %s\n", SDL_GetError());
		pApp->m_pRenderer=SDL_CreateRenderer(pApp->m_pWindow, -1, SDL_RENDERER_SOFTWARE);
		if(pApp->m_pRenderer==NULL){
			fprintf(stderr,"Failed to create software renderer: %s\n", SDL_GetError());
			mBitsSet(pApp->m_uStatus, ST_APP_INIT_FAILED);
			return pApp;
		}
		fprintf(stderr,"Created software renderer instead!\n");
	}
	return pApp;
}

/**
 * @brief App "destructor".
 *        Performs the app destruction by freeing the memory allocated
 *        by the "constructor".
 * 
 * @param pApp pointer to the app entity to destroy.
 * @return t_app* NULL.
 */
t_app*AppDel(t_app*pApp){
	assert(pApp);
	if(pApp->m_timerID) 	SDL_RemoveTimer(pApp->m_timerID);


#ifdef TEST_ISOLATE_ENTITY_STAR
	/** ---------FOR TESTING PURPOSE-----------
	 * @todo Destroy here the two star entities
	 */
	StarDel(pApp->pStarOne);
	StarDel(pApp->pStarTwo);
#endif

	/**
	 * @todo Destroying the star container.
	 */ 

	ContainerDel(pApp->m_pStars);

	if(pApp->m_pRenderer) 	SDL_DestroyRenderer(pApp->m_pRenderer);
	if(pApp->m_pWindow)		SDL_DestroyWindow(pApp->m_pWindow);

	if(mIsBitsSet(pApp->m_uStatus, ST_APP_SDL_INITIATED)) SDL_Quit();

	free(pApp);
	return NULL;
}

/**
 * @brief The application event loop.
 * 
 * @param pApp pointer to the app entity to run.
 * @return int EXIT_SUCCESS in case of a normal issue,
 *             EXIT_FAILURE otherwise.
 */
int AppRun(t_app*pApp){
	assert(pApp);
	if(mIsBitsSet(pApp->m_uStatus, ST_APP_INIT_FAILED)) return EXIT_FAILURE;

	pApp->m_timerID=SDL_AddTimer(APP_ANIMATE_RATE, (SDL_TimerCallback)_AppTimerCallback, pApp);

	int iQuit=0;
	SDL_Event event;


#ifdef TEST_ISOLATE_ENTITY_STAR
	/** ---------FOR TESTING PURPOSE-----------
	 * @todo Create here two star entities
	 */


	pApp->pStarOne = StarNew((SDL_Point){rand()%(APP_WINDOW_WIDTH - APP_SPACE_PADDING*2)+APP_SPACE_PADDING,rand()%(APP_WINDOW_HEIGHT - APP_SPACE_PADDING*2)+APP_SPACE_PADDING},
							(SDL_Point){rand()%(APP_STAR_SPEED_MAX)-APP_STAR_SPEED_MAX,rand()%(APP_STAR_SPEED_MAX)-APP_STAR_SPEED_MAX},
							rand()%(APP_STAR_NB_BRANCHES_MAX-APP_STAR_NB_BRANCHES_MIN)+APP_STAR_NB_BRANCHES_MIN,
							rand()%( APP_STAR_RADIUS_MAX- APP_STAR_RADIUS_MIN)+ APP_STAR_RADIUS_MIN,
							(SDL_Color){rand()%(255-100)+100,rand()%(255-100)+100,rand()%(255-100)+100,rand()%(255-100)+100});



	pApp->pStarTwo = StarNew((SDL_Point){rand()%(APP_WINDOW_WIDTH - APP_SPACE_PADDING*2)+APP_SPACE_PADDING,rand()%(APP_WINDOW_HEIGHT - APP_SPACE_PADDING*2)+APP_SPACE_PADDING},
							(SDL_Point){rand()%(APP_STAR_SPEED_MAX)-APP_STAR_SPEED_MAX,rand()%(APP_STAR_SPEED_MAX)-APP_STAR_SPEED_MAX},
							rand()%(APP_STAR_NB_BRANCHES_MAX-APP_STAR_NB_BRANCHES_MIN)+APP_STAR_NB_BRANCHES_MIN,
							rand()%( APP_STAR_RADIUS_MAX- APP_STAR_RADIUS_MIN)+ APP_STAR_RADIUS_MIN,
							(SDL_Color){rand()%(255-100)+100,rand()%(255-100)+100,rand()%(255-100)+100,rand()%(255-100)+100});


#endif


	while(!iQuit){
		while(SDL_PollEvent(&event)){
			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				/**
				 * @todo Taking care of mouse button goes down.
				 */
				mBitsSet(pApp->m_uStatus,ST_APP_MOUSE_BUTTON_DOWN);
				pApp->m_ptStart.x = event.button.x;
				pApp->m_ptStart.y = event.button.y;
				break;
			case SDL_MOUSEBUTTONUP:
				/**
				 * @todo Taking care of mouse button goes up.
				 */
				mBitsClr(pApp->m_uStatus,ST_APP_MOUSE_BUTTON_DOWN);

				break;
			case SDL_MOUSEMOTION:
				/**
				 * @todo Taking care of mouse motion.
				 */

				pApp->m_generateOrigin.x = event.motion.x;
				pApp->m_generateOrigin.y = event.motion.y;


					/** ---------FOR TESTING PURPOSE-----------
					 * @todo Offseting here the two star entities
					 */
					if(mIsBitsSet(pApp->m_uStatus,ST_APP_MOUSE_BUTTON_DOWN)){
						pApp->m_ptStart.x = event.motion.x-pApp->m_ptStart.x;
						pApp->m_ptStart.y = event.motion.y-pApp->m_ptStart.y;
#ifdef TEST_ISOLATE_ENTITY_STAR
						StarOffset(pApp->pStarOne, &pApp->m_ptStart);
						StarOffset(pApp->pStarTwo, &pApp->m_ptStart);
#endif
						ContainerParse(pApp->m_pStars,(t_ptfVV)StarOffset, &pApp->m_ptStart);
						pApp->m_ptStart.x = event.motion.x;
						pApp->m_ptStart.y = event.motion.y;
					}

				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_SPACE:
					/**
					 * @todo Taking care of forcing star generation.
					 */
//					ContainerPushback(pApp->m_pStars,StarNew((SDL_Point){rand()%(APP_WINDOW_WIDTH - APP_SPACE_PADDING*2)+APP_SPACE_PADDING,rand()%(APP_WINDOW_HEIGHT - APP_SPACE_PADDING*2)+APP_SPACE_PADDING},
//							(SDL_Point){rand()%(APP_STAR_SPEED_MAX*2)-APP_STAR_SPEED_MAX,rand()%(APP_STAR_SPEED_MAX*2)-APP_STAR_SPEED_MAX},
//							rand()%(APP_STAR_NB_BRANCHES_MAX-APP_STAR_NB_BRANCHES_MIN)+APP_STAR_NB_BRANCHES_MIN,
//							rand()%( APP_STAR_RADIUS_MAX- APP_STAR_RADIUS_MIN)+ APP_STAR_RADIUS_MIN,
//							(SDL_Color){rand()%(255-100)+100,rand()%(255-100)+100,rand()%(255-100)+100,rand()%(255-100)+100}));
					pApp->m_generateTimer = 0;

					break;
				case SDLK_f:
					/**
					 * @todo Taking care of flushing the star container.
					 */
					ContainerFlush(pApp->m_pStars);
					break;
				case SDLK_p:
					/**
					 * @todo Taking care of animation paused/unpaused.
					 */
					mBitsTgl(pApp->m_uStatus,ST_APP_PAUSED);
					break;
				case SDLK_ESCAPE:
					event.type=SDL_QUIT;
					SDL_PushEvent(&event);
					break;
				default:
					break;
				}
				break;
			case SDL_QUIT:
				iQuit=1;
				break;
			default:
				break;
			}
		}
	}

	return EXIT_SUCCESS;
}

/**
 * @brief Animation callback function prototype.
 * 		  Performs all tasks for animating and drawing graphical objects.
 * 
 * @param interval number of milliseconds between two call back.
 * @param pApp pointer to the application structure.
 * @return Uint32 interval in milliseconds for the next call back.
 */
Uint32 _AppTimerCallback(Uint32 interval, t_app*pApp){
	if(mIsBitsSet(pApp->m_uStatus, ST_APP_PAUSED)) return interval;
	SDL_SetRenderDrawColor(
			pApp->m_pRenderer,
			APP_WINDOW_COLOR_BACKGND.r,
			APP_WINDOW_COLOR_BACKGND.g,
			APP_WINDOW_COLOR_BACKGND.b,
			APP_WINDOW_COLOR_BACKGND.a);

	SDL_RenderClear(pApp->m_pRenderer);


	/**
	 * @todo Taking care of displaying space simulation area.
	 */
	SDL_SetRenderDrawColor(pApp->m_pRenderer, APP_WINDOW_COLOR_AREA.r,
												APP_WINDOW_COLOR_AREA.g,
												APP_WINDOW_COLOR_AREA.b,
												APP_WINDOW_COLOR_AREA.a);

	SDL_RenderDrawRect(pApp->m_pRenderer, &pApp->m_space);


	/**
	 * @todo Taking care of displaying star generation area.
	 */

	SDL_Rect generate = {pApp->m_generateOrigin.x -APP_STAR_AREA_GEN_SIZE/2,
						pApp->m_generateOrigin.y - APP_STAR_AREA_GEN_SIZE/2,
						APP_STAR_AREA_GEN_SIZE,
						APP_STAR_AREA_GEN_SIZE};



	SDL_SetRenderDrawColor(pApp->m_pRenderer, 250, 200, 20, 255);
	SDL_RenderDrawRect(pApp->m_pRenderer, &generate);
	//APP_STAR_AREA_GEN_SIZE


	/**
	 * @todo Taking care of displaying number of stars located in simulation.
	 */
	 char buf[128];
	 sprintf(buf,"STARS NUMBER : %d  TIME BEFORE MILKY WAY : %d",(int) ContainerCard(pApp->m_pStars),pApp->m_generateTimer);
	 SDL_SetWindowTitle(pApp->m_pWindow, buf);



#ifdef TEST_ISOLATE_ENTITY_STAR
	/** ---------FOR TESTING PURPOSE-----------
	 * @todo Drawing here the two star entities
	 */

	StarDraw(pApp->pStarOne, pApp->m_pRenderer);
	StarDraw(pApp->pStarTwo, pApp->m_pRenderer);

	/** ---------FOR TESTING PURPOSE-----------
	 * @todo Moving here the two star entities
	 */
	StarMove(pApp->pStarOne, &pApp->m_space);
	StarMove(pApp->pStarTwo, &pApp->m_space);

#endif



	/**
	 * @todo Taking care of drawing stars located in simulation.
	 */

	ContainerParse(pApp->m_pStars, (t_ptfVV)StarDraw, pApp->m_pRenderer);



	/**
	 * @todo Taking care of moving stars located in simulation.
	 */

	ContainerParseDelIf(pApp->m_pStars, (t_ptfVV)StarMove, &pApp->m_space);



	/**
	 * @todo Taking care of generation of new stars to place in simulation.
	 */
	pApp->m_generateTimer--;
	if(pApp->m_generateTimer <= 0){
		int i = 0;

		while(i < rand()%( APP_STAR_NB_MAX- APP_STAR_NB_MIN)+ APP_STAR_NB_MIN){
			ContainerPushback(pApp->m_pStars,StarNew((SDL_Point){pApp->m_generateOrigin.x + rand()%(APP_STAR_AREA_GEN_SIZE +1)-APP_STAR_AREA_GEN_SIZE/2,
																pApp->m_generateOrigin.y + rand()%(APP_STAR_AREA_GEN_SIZE +1)-APP_STAR_AREA_GEN_SIZE/2},
										(SDL_Point){rand()%(APP_STAR_SPEED_MAX*2)-APP_STAR_SPEED_MAX,rand()%(APP_STAR_SPEED_MAX*2)-APP_STAR_SPEED_MAX},
										rand()%(APP_STAR_NB_BRANCHES_MAX-APP_STAR_NB_BRANCHES_MIN)+APP_STAR_NB_BRANCHES_MIN,
										rand()%( APP_STAR_RADIUS_MAX- APP_STAR_RADIUS_MIN)+ APP_STAR_RADIUS_MIN,
										(SDL_Color){rand()%(255-100)+100,rand()%(255-100)+100,rand()%(255-100)+100,rand()%(255-100)+100}));
			i++;
		}
		i=0;
		pApp->m_generateTimer =rand()%(APP_STAR_TIMER_GEN_MAX-APP_STAR_TIMER_GEN_MIN)+APP_STAR_TIMER_GEN_MIN;
	}




	SDL_RenderPresent(pApp->m_pRenderer);
	return interval;
}


