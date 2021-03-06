/*=====================================================================

QGroundControl Open Source Ground Control Station

(c) 2009, 2010 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>

This file is part of the QGROUNDCONTROL project

    QGROUNDCONTROL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QGROUNDCONTROL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QGROUNDCONTROL. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
 * @file
 *   @brief Definition of class UASManager
 *   @author Lorenz Meier <mavteam@student.ethz.ch>
 *
 */

#ifndef _UASMANAGER_H_
#define _UASMANAGER_H_

#include <QThread>
#include <QList>
#include <QMutex>
#include <UASInterface.h>

/**
 * @brief Central manager for all connected aerial vehicles
 *
 * This class keeps a list of all connected / configured UASs. It also stores which
 * UAS is currently select with respect to user input or manual controls.
 **/
class UASManager : public QThread
{
    Q_OBJECT

public:
    static UASManager* instance();
    ~UASManager();

    void run();
    /**
     * @brief Get the currently selected UAS
     *
     * @return NULL pointer if no UAS exists, active UAS else
     **/
    UASInterface* getActiveUAS();
    UASInterface* silentGetActiveUAS();
    /**
     * @brief Get the UAS with this id
     *
     * Although not enforced by this implementation, the IDs are constrained to be
     * in the range of 1 - 127 by the MAVLINK protocol.
     *
     * @param id unique system / aircraft id
     * @return UAS with the given ID, NULL pointer else
     **/
    UASInterface* getUASForId(int id);

    QList<UASInterface*> getUASList();


public slots:

    /**
     * @brief Add a new UAS to the list
     *
     * This command will only be executed if this UAS does not yet exist.
     * @param UAS unmanned air system to add
     **/
    void addUAS(UASInterface* UAS);


    /**
      * @brief Set a UAS as currently selected
      *
      * @param UAS Unmanned Air System to set
      **/
    void setActiveUAS(UASInterface* UAS);


    /**
     * @brief Launch the active UAS
     *
     * The groundstation has always one Unmanned Air System selected.
     * All commands are executed on the UAS in focus. This command starts
     * the launch sequence.
     *
     * @return True if the UAS could be launched, false else
     */
    bool launchActiveUAS();

    bool haltActiveUAS();

    bool continueActiveUAS();

    /**
     * @brief Land the active UAS
     *
     * The groundstation has always one Unmanned Air System selected.
     * All commands are executed on the UAS in focus. This command starts
     * the land sequence. Depending on the onboard control, this could mean
     * returning to the landing spot as well as descending on the current
     * position.
     *
     * @return True if the UAS could be landed, false else
     */
    bool returnActiveUAS();


    /**
     * @brief EMERGENCY: Stop active UAS
     *
     * The groundstation has always one Unmanned Air System selected.
     * All commands are executed on the UAS in focus. This command
     * starts an emergency landing. Depending on the onboard control,
     * this usually means descending rapidly on the current position.
     *
     * @warning This command can severely damage the UAS!
     *
     * @return True if the UAS could be landed, false else
     */
    bool stopActiveUAS();

    /**
     * @brief EMERGENCY: Kill active UAS
     *
     * The groundstation has always one Unmanned Air System selected.
     * All commands are executed on the UAS in focus. This command
     * shuts off all onboard motors immediately. This leads to a
     * system crash, but might prevent external damage, e.g. to people.
     * This command is secured by an additional popup message window.
     *
     * @warning THIS COMMAND RESULTS IN THE LOSS OF THE SYSTEM!
     *
     * @return True if the UAS could be landed, false else
     */
    bool killActiveUAS();

    /**
     * @brief Configure the currently active UAS
     *
     * This command will bring up the configuration dialog for the particular UAS.
     */
    void configureActiveUAS();

    /** @brief Shut down the onboard operating system down */
    bool shutdownActiveUAS();


protected:
    UASManager();
    QMap<int, UASInterface*> systems;
    UASInterface* activeUAS;
    QMutex activeUASMutex;

signals:
    void UASCreated(UASInterface* UAS);
    /** @brief The UAS currently under main operator control changed */
    void activeUASSet(UASInterface* UAS);
    /** @brief The UAS currently under main operator control changed */
    void activeUASSet(int systemId);
    /** @brief The UAS currently under main operator control changed */
    void activeUASStatusChanged(UASInterface* UAS, bool active);
    /** @brief The UAS currently under main operator control changed */
    void activeUASStatusChanged(int systemId, bool active);

};

#endif // _UASMANAGER_H_
