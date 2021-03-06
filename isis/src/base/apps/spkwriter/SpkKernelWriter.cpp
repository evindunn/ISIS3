/**                                                                       
 * @file                                                                  
 * $Revision: 5850 $
 * $Date: 2014-05-27 15:22:24 -0700 (Tue, 27 May 2014) $
 * $Id: SpkKernelWriter.cpp 5850 2014-05-27 22:22:24Z jwbacker@GS.DOI.NET $
 * 
 *   Unless noted otherwise, the portions of Isis written by the USGS are 
 *   public domain. See individual third-party library and package descriptions 
 *   for intellectual property information, user agreements, and related  
 *   information.                                                         
 *                                                                        
 *   Although Isis has been used by the USGS, no warranty, expressed or   
 *   implied, is made by the USGS as to the accuracy and functioning of such 
 *   software and related material nor shall the fact of distribution     
 *   constitute any such warranty, and no responsibility is assumed by the
 *   USGS in connection therewith.                                        
 *                                                                        
 *   For additional information, launch                                   
 *   $ISISROOT/doc//documents/Disclaimers/Disclaimers.html                
 *   in a browser or see the Privacy &amp; Disclaimers page on the Isis website,
 *   http://isis.astrogeology.usgs.gov, and the USGS privacy and disclaimers on
 *   http://www.usgs.gov/privacy.html.                                    
 */
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "Application.h"
#include "SpkKernelWriter.h"
#include "Pvl.h"
#include "IString.h"
#include "IException.h"

#include "NaifStatus.h"

using namespace std;

namespace Isis {

  /**
   * @brief Provide general comments for an SPK kernel header
   * 
   * 
   * @return QString Comment generated for SPK SPICE kernel header 
   *  
   * @internal 
   * @history 2012-05-23 Kris Becker Added creation of special tag to ID the 
   *                                 kernel as needing special light time and
   *                                 stellar aberration correction in spiceinit
   */
  QString SpkKernelWriter::k_header(const QString &comfile) const {
    ostringstream comment;
    comment << "\
 ****************************************************************************\n\
   USGS ISIS (spkwriter) Generated SPK Kernel\n\
   Created By:   " << Application::UserName() << "\n\
   Date Created: " << Application::DateTime() << "\n\
 ****************************************************************************\n\
 \n\
 \n\
 Position Data in the File\n\
 -----------------------------------------------------------------------\n\
 \n\
       This file contains time ordered array of geometric states \n\
       (kilometers) and rates of change (kilometers/second) of body\n\
       relative to center, specified relative to frame.\n\
 \n\
 \n\
 Status\n\
 -----------------------------------------------------------------------\n\
 \n\
       This kernel was generated for the purposes of storing C-Smithed\n\
       position updates that may have been generated from ISIS processing\n\
       techniques (controlled imaging, jitter analysis, etc...).  These\n\
       SPK kernels are intended to mimick SPKs provided by individual\n\
       missions (or NAV teams).\n\
 \n\
 \n\
 Pedigree\n\
 -----------------------------------------------------------------------\n\
 \n\
       This file was generated by an automated process.  The ISIS\n\
       application spkwriter was used to read SPK kernel data\n\
       contained within an ISIS cube file.  It then writes it as an\n\
       individual segment in the SPK.  Hence, a list of files can be\n\
       written to a single SPK kernel.  However, mixing the instruments\n\
       contained in a single SPK kernel is discouraged.\n\
 \n\
 \n\
 Angular Rates\n\
 -----------------------------------------------------------------------\n\
 \n\
       This kernel typically contains state vectors of rates of change\n\
       as a function of time but may only contain position vectors.  The\n\
       ephemeris given is for the body moving relative to the center of\n\
       motion.\n\
 \n\
 \n\
 Usage Note\n\
 -----------------------------------------------------------------------\n\
 \n\
       To make use of this file in a typical SPICE based application,\n\
       users must supply at least a leapseconds kernel. This file is\n\
       necessary for time-based conversions.  They should be the same\n\
       kernels that were originally used to initialize the image.\n\
 \n\
       Segments in this file are actually individual ISIS files where the\n\
       internally cached SPICE data is extracted and transformed into the\n\
       appropriate content to satisfy NAIF\'s SPICE kernel storage\n\
       requirements.\n\
 \n\
       Note that ISIS defaults to applying light time and stellar\n\
       abberation correction when computing positions relative to s/c and\n\
       target body.  Currently, this correction should not be utilized\n\
       for kernels created by the spkwriter application.  Therefore the\n\
       computation correcting for light time/stellar abberation is turned\n\
       off. It should be noted that this option applies to all files\n\
       contained herein.  (ID:USGS_SPK_ABCORR=NONE)\n\
\n\
        The contents of this kernel are summarized below.\n\
\n\
User Comments\n\
-----------------------------------------------------------------------\n\
\n";

    // Now write any user comments provided
   if ( !comfile.isEmpty() ) {

     // Write user comment header
     TextFile txt(comfile);
     QString cline;
     while ( txt.GetLineNoFilter(cline )) {
       comment << cline << "\n";
     }
   }
   else {
     // None provided
     comment << "\
      NONE\n";
   }

   //  Finish comments for segement data
   comment << "\
 \n\
 Segment (by file) Summary\n\
 -----------------------------------------------------------------------\n\
 \n\
       The follow sections describe each segment in this SPK kernel.  Each\n\
       segment is a file in the input list.  When running ISIS spiceinit,\n\
       the kernels listed for each file should be supplied to ensure proper\n\
       geometry can be reproduced accurately.\n\
 \n";

    return (QString(comment.str().c_str()));
 }




};  // namespace Isis


