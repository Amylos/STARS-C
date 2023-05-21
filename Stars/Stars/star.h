/**
 * @file: 	star.h
 *
 * @date: 	<date>
 * @author: <author>
 *
 */

#ifndef STAR_H_
#define STAR_H_
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define	STAR_INTERNAL_RADIUS_RATIO		(.45)


/**
 * @brief The generic type for a star entity
 *        Opaque structre.
 */
typedef struct s_star t_star;

/**
 * @brief Star "constructor".
 *        Performs a star entity creation by allocating memory need by
 *        the entity to store its parameters. Initializes the star
 *        parameters with parameters passed to the function.
 * 
 * @param [in] ptCenter the point locating the star to create.
 * @param [in] ptVelocity the velocity or speed of the star.
 * @param [in] szBranches the number of branches of the star.
 * @param [in] iRadius the external radius of the star.
 * @param [in] color the RGBA color of the star.
 * @return t_star* the pointer to the newly created star.
 */
t_star*StarNew(SDL_Point ptCenter, SDL_Point ptVelocity, size_t szBranches, int iRadius, SDL_Color color);

/**
 * @brief Star "destructor".
 *        Performs the star destruction by freeing the memory allocated
 *        by the "constructor".
 * 
 * @param [in] pStar pointer to the star entity to destroy.
 * @return t_star* NULL. 
 */
t_star*StarDel(t_star*pStar);

/**
 * @brief Performs drawing of the star entity.
 * 
 * @param [in] pStar pointer to the star entity to be drawn.
 * @param [in] pRenderer pointer to the renderer needed for drawing.
 * @return t_star* NULL.
 */
t_star*StarDraw(const t_star*pStar, SDL_Renderer*pRenderer);

/**
 * @brief Performs the moving of the star entity.
 *        Moving takes place into an area: the space area.
 *        This function must return a non null value in case
 *        of over-crossing the space area limits.
 * 
 * @param [in] pStar pointer to the star entity to be moved.
 * @param [in] pSpace pointer to structure delimiting the space area.
 * @return t_star* a Non Null value in case of over-crossing,
 *                 NULL otherwise.
 */
t_star*StarMove(t_star*pStar, SDL_Rect*pSpace);

/**
 * @brief Performs location offset of the star entity.
 *        Moving the star entity location by the offset
 *        value passed to the function.  
 * 
 * @param [in] pStar pointer to the star entity to which to make offset.
 * @param [in] pOffset value of the offset to apply.
 * @return t_star* NULL.
 */
t_star*StarOffset(t_star*pStar, SDL_Point*pOffset);


#ifdef __cpluplus
}
#endif
