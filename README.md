# LEIUMS
**Laboratory Equipment Inventory and Usage Management System (LEIUMS)** <br><br>
&nbsp;&nbsp;&nbsp;&nbsp;The Laboratory Equipment Inventory and Usage Management System is a C++-based program developed to efficiently manage, track, and analyze laboratory equipment data. It records essential details such as equipment name, quantity, acquisition date, and condition, while performing computations related to usage hours, depreciation, and availability rates. The system assists laboratory personnel in maintaining accurate inventory records, monitoring equipment lifespan, and planning timely repairs or replacements. By integrating programming logic, computation, and file handling, this project demonstrates the engineering application of structured programming for optimizing laboratory resources.

# 31-10-2025 v1
  - Program prototype <br>
  - Added main menu function <br>
  - Started some functions for the menu options (some aren't working) <br>
# 01-11-2025 v1.2
  - Working display data <br>
  - Working add data <br>
  - Working save data <br>
  - Added empty input detector in name <br>
  - Added empty input detector in quantity <br>
  - Added empty input detector in date <br>
# 02-11-2025 v1.3
  - Display can handle more than 15 data <br>
  - Data type validation function (int n float) <br>
  - Add data input validation <br>
  - Add data cancel feature <br>
  - Add data empty input validation <br>
  - Validated data storing <br>
# 03-11-2025 v1.4
  - Name input length validation <br>
  - Changed "cance" to "icancel" to allow name inputs like "can" <br>
  - Hours does not allow negative inputs <br>
  - Depriciation does not allow negative and over 100 inputs <br>
  - Availability does not allow negative and over 100 inputs <br>
  - Added independent load data function for usability <br>
  - Display and save function remodel to match load function <br>
  - Search data added (only by name for now) <br>
  - More search options coming soon...
  - Remove data coming soon...
# 04-11-2025 v1.4.2
  - Added paginated display (15 data per page) with left/right arrow navigation <br>
  - Improved equipment display layout (aligned columns, header, and footer) <br>
  - Added formatted date display (DD-MM-YYYY with leading zeros) <br>
  - Unified color scheme and improved interface consistency <br>
  - Improved search results layout to match new display format <br>
  - Optimized data loading and validation logic for consistency <br>
  - Lowercase normalization for condition and status inputs <br>
  - Added delete functionality inside search results (via DEL key) <br>
  - Improved overall reliability when updating or removing equipment data <br>
  - Minor code cleanup and structural improvements for maintainability <br>
# 04-11-2025 v1.5.1
  - Removed quantity and depreciation fields from Equipment <br>
  - Introduced efficiency, status, and category fields <br>
  - Reduced max_equipments to 500 for memory management <br>
  - Implemented record deletion within search <br>
  - Added keyboard navigation (↑ ↓ ← →, DEL, ESC) <br>
  - Added search by name filtering <br>
  - Updated file format to match new structure fields <br>
  - Updated saveAllEquipment() to fully overwrite with the new field order <br>
  - Rewritten loadEquipment() for safer token parsing <br>
  - Page counter and data range display <br>
  - Navigation with left/right arrows <br>
  - Unified error(), info(), and success() message formatting <br>
  - Added returnMenu() hint prompts <br>
  - Cleaner color and spacing layout for all screens <br>
# 04-11-2025 v1.5.2
  - Reduced max_equipments from 500 → 350 for better stability <br>
  - Added automatic efficiency, availability, and category computation based on status and usageHours <br>
  - Implemented dynamic availability logic (e.g., available = 100%, maintenance = 50%, etc.) <br>
  - Added bordered dialog with all equipment info <br>
  - Activated via ENTER key from search results <br>
  - Now shows only key data (Name, Status, Condition, Efficiency %, Avail %) <br>
