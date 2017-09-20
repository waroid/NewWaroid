/*
 * MainManager.h
 *
 *  Created on: 2015. 2. 13.
 *      Author: mirime
 */

#ifndef MAINMANAGER_H_
#define MAINMANAGER_H_


class MainManager
{
public:
	MainManager();
	virtual ~MainManager();

public:
	bool start();
	void stop();
};

#endif /* MAINMANAGER_H_ */
