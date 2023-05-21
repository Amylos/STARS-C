/**
 * @file: 	app.h
 *
 * @date: 	<date>
 * @author: <author>
 *
 */

#define APP_WINDOW_TITLE_STR		("Stars expansion")
#define APP_WINDOW_WIDTH			(800)
#define APP_WINDOW_HEIGHT			(800)
#define APP_WINDOW_COLOR_BACKGND	(SDL_Color){30,30,45,255}
#define APP_WINDOW_COLOR_AREA   	(SDL_Color){120,120,180,255}
#define APP_SPACE_PADDING			(60)

#define APP_ANIMATE_RATE			(40)

#define APP_STAR_NB_MAX				(80)
#define APP_STAR_NB_MIN				(20)

#define APP_STAR_NB_BRANCHES_MAX	(10)
#define APP_STAR_NB_BRANCHES_MIN	(4)

#define APP_STAR_RADIUS_MAX			(25)
#define APP_STAR_RADIUS_MIN			(5)

#define APP_STAR_SPEED_MAX			(15)

#define APP_STAR_RGB_MAX			(255)
#define APP_STAR_RGB_MIN			(100)

#define APP_STAR_TIMER_GEN_MAX		(100)
#define APP_STAR_TIMER_GEN_MIN		(50)

#define APP_STAR_AREA_GEN_SIZE		(200)

/**
 * @brief  The generic type for an app entity
 *         Opaque structure.
 */
typedef struct s_app t_app;

/**
 * @brief App "constructor".
 *        Performs an app entity creation by allocating memory need by
 *        the app to store its parameters. Initializes the app
 *        parameters by creating all necessary components.
 * 
 * @return t_app* the pointer to the newly created app.
 */
t_app*AppNew(void);

/**
 * @brief App "destructor".
 *        Performs the app destruction by freeing the memory allocated
 *        by the "constructor".
 * 
 * @param pApp pointer to the app entity to destroy.
 * @return t_app* NULL.
 */
t_app*AppDel(t_app*pApp);

/**
 * @brief The application event loop.
 * 
 * @param pApp pointer to the app entity to run.
 * @return int EXIT_SUCCESS in case of a normal issue,
 *             EXIT_FAILURE otherwise.
 */
int AppRun(t_app*pApp);
