/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

#include <cgi/map/cgi_Layers.h>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>

#include <cgi/cgi_Geometry.h>
#include <cgi/cgi_Mercator.h>
#include <cgi/cgi_Models.h>

#include <cgi/map/cgi_Map.h>

#include <fdm/fdm_Path.h>
#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlDoc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Layers::Layers( Module *parent ) :
    Module( parent )
{
    m_switchCrops     = new osg::Switch();
    m_switchGrassland = new osg::Switch();
    m_switchWoodland  = new osg::Switch();
    m_switchBuiltup   = new osg::Switch();
    m_switchRailroads = new osg::Switch();
    m_switchRoads     = new osg::Switch();
    m_switchAirports  = new osg::Switch();
    m_switchSatellite = new osg::Switch();
    m_switchBorders   = new osg::Switch();

    m_oceans       = new osg::PositionAttitudeTransform();
    m_landmass     = new osg::PositionAttitudeTransform();
    m_coastline    = new osg::PositionAttitudeTransform();
    m_crops        = new osg::PositionAttitudeTransform();
    m_grassland    = new osg::PositionAttitudeTransform();
    m_woodland     = new osg::PositionAttitudeTransform();
    m_builtup      = new osg::PositionAttitudeTransform();
    m_railroads    = new osg::PositionAttitudeTransform();
    m_roads        = new osg::PositionAttitudeTransform();
    m_airports     = new osg::PositionAttitudeTransform();
    m_water_course = new osg::PositionAttitudeTransform();
    m_water_inland = new osg::PositionAttitudeTransform();
    m_satellite    = new osg::PositionAttitudeTransform();
    m_borders      = new osg::PositionAttitudeTransform();

    m_oceans       ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zOceans    ) );
    m_landmass     ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zLandmass  ) );
    m_coastline    ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zCoastline ) );
    m_crops        ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zCrops     ) );
    m_grassland    ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zGrassland ) );
    m_woodland     ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zWoodland  ) );
    m_builtup      ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zBuiltup   ) );
    m_railroads    ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zRailroads ) );
    m_roads        ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zRoads     ) );
    m_airports     ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zAirports  ) );
    m_water_course ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zWaterCourse ) );
    m_water_inland ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zWaterInland ) );
    m_satellite    ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zSatellite ) );
    m_borders      ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zBorders   ) );

    m_switchCrops     ->addChild( m_crops     .get() );
    m_switchGrassland ->addChild( m_grassland .get() );
    m_switchWoodland  ->addChild( m_woodland  .get() );
    m_switchBuiltup   ->addChild( m_builtup   .get() );
    m_switchRailroads ->addChild( m_railroads .get() );
    m_switchRoads     ->addChild( m_roads     .get() );
    m_switchAirports  ->addChild( m_airports  .get() );
    m_switchSatellite ->addChild( m_satellite .get() );
    m_switchBorders   ->addChild( m_borders   .get() );

    m_root->addChild( m_oceans       .get() );
    m_root->addChild( m_landmass     .get() );
    m_root->addChild( m_coastline    .get() );
    m_root->addChild( m_water_course .get() );
    m_root->addChild( m_water_inland .get() );

    m_root->addChild( m_switchCrops     .get() );
    m_root->addChild( m_switchGrassland .get() );
    m_root->addChild( m_switchWoodland  .get() );
    m_root->addChild( m_switchBuiltup   .get() );
    m_root->addChild( m_switchRailroads .get() );
    m_root->addChild( m_switchRoads     .get() );
    m_root->addChild( m_switchAirports  .get() );
    m_root->addChild( m_switchSatellite .get() );
    m_root->addChild( m_switchBorders   .get() );

    initLayer( m_oceans       .get() , Map::colorOceans      );
    initLayer( m_landmass     .get() , Map::colorLandmass    );
    initLayer( m_coastline    .get() , Map::colorCoastline   );
    initLayer( m_crops        .get() , Map::colorCrops       );
    initLayer( m_grassland    .get() , Map::colorGrassland   );
    initLayer( m_woodland     .get() , Map::colorWoodland    );
    initLayer( m_builtup      .get() , Map::colorBuiltup     );
    initLayer( m_railroads    .get() , Map::colorRailroads   );
    initLayer( m_roads        .get() , Map::colorRoads       );
    initLayer( m_airports     .get() , Map::colorAirports    );
    initLayer( m_water_course .get() , Map::colorWaterCourse );
    initLayer( m_water_inland .get() , Map::colorWaterInland );
    initLayer( m_borders      .get() , Map::colorBorders     );

    createOcean();

#   ifndef SIM_MARBLE_MAPS
    readLayers();
#   endif
}

////////////////////////////////////////////////////////////////////////////////

Layers::~Layers() {}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityCrops( bool visible )
{
    if ( visible )
        m_switchCrops->setAllChildrenOn();
    else
        m_switchCrops->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityGrassland( bool visible )
{
    if ( visible )
        m_switchGrassland->setAllChildrenOn();
    else
        m_switchGrassland->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityWoodland( bool visible )
{
    if ( visible )
        m_switchWoodland->setAllChildrenOn();
    else
        m_switchWoodland->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityBuiltup( bool visible )
{
    if ( visible )
        m_switchBuiltup->setAllChildrenOn();
    else
        m_switchBuiltup->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityRailroads( bool visible )
{
    if ( visible )
        m_switchRailroads->setAllChildrenOn();
    else
        m_switchRailroads->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityRoads( bool visible )
{
    if ( visible )
        m_switchRoads->setAllChildrenOn();
    else
        m_switchRoads->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityAirports( bool visible )
{
    if ( visible )
        m_switchAirports->setAllChildrenOn();
    else
        m_switchAirports->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilitySatellite( bool visible )
{
    if ( visible )
        m_switchSatellite->setAllChildrenOn();
    else
        m_switchSatellite->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityBorders( bool visible )
{
    if ( visible )
        m_switchBorders->setAllChildrenOn();
    else
        m_switchBorders->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::createOcean()
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    m_oceans->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    geode->addDrawable( geometry.get() );

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

    v->push_back( osg::Vec3( -Mercator::max_x, -Mercator::max_y, 0.0 ) );
    v->push_back( osg::Vec3(  Mercator::max_x, -Mercator::max_y, 0.0 ) );
    v->push_back( osg::Vec3(  Mercator::max_x,  Mercator::max_y, 0.0 ) );
    v->push_back( osg::Vec3( -Mercator::max_x,  Mercator::max_y, 0.0 ) );

    Geometry::createQuad( geometry.get(), v.get() );
}

////////////////////////////////////////////////////////////////////////////////

void Layers::initLayer( osg::Node* layer, osg::Vec3 color, float width )
{
    osg::ref_ptr<osg::Material> material = new osg::Material();
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAmbient( osg::Material::FRONT, osg::Vec4( color, 1.0f ) );
    material->setDiffuse( osg::Material::FRONT, osg::Vec4( color, 1.0f ) );
    layer->getOrCreateStateSet()->setAttribute( material.get(),
            osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON );

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth( width );
    layer->getOrCreateStateSet()->setAttributeAndModes( lineWidth.get(),
        osg::StateAttribute::ON );
}

////////////////////////////////////////////////////////////////////////////////

void Layers::readLayers()
{
    fdm::XmlDoc doc( fdm::Path::get( "data/map/layers/layers.xml" ) );

    if ( doc.isOpen() )
    {
        fdm::XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            if ( 0 == fdm::String::icompare( rootNode.getName(), "layers" ) )
            {
                fdm::XmlNode landmassNode    = rootNode.getFirstChildElement( "landmass"     );
                fdm::XmlNode coastlineNode   = rootNode.getFirstChildElement( "coastline"    );
                fdm::XmlNode cropsNode       = rootNode.getFirstChildElement( "crops"        );
                fdm::XmlNode grasslandNode   = rootNode.getFirstChildElement( "grassland"    );
                fdm::XmlNode woodlandNode    = rootNode.getFirstChildElement( "woodland"     );
                fdm::XmlNode builtupNode     = rootNode.getFirstChildElement( "builtup"      );
                fdm::XmlNode railroadsNode   = rootNode.getFirstChildElement( "railroads"    );
                fdm::XmlNode roadsNode       = rootNode.getFirstChildElement( "roads"        );
                fdm::XmlNode airportsNode    = rootNode.getFirstChildElement( "airports"     );
                fdm::XmlNode waterCourseNode = rootNode.getFirstChildElement( "water_course" );
                fdm::XmlNode waterInlandNode = rootNode.getFirstChildElement( "water_inland" );
                fdm::XmlNode satelliteNode   = rootNode.getFirstChildElement( "satellite"    );
                fdm::XmlNode bordersNode     = rootNode.getFirstChildElement( "borders"      );

                if ( landmassNode    .isValid() ) readLayer( landmassNode    , m_landmass     .get() );
                if ( coastlineNode   .isValid() ) readLayer( coastlineNode   , m_coastline    .get() );
                if ( cropsNode       .isValid() ) readLayer( cropsNode       , m_crops        .get() );
                if ( grasslandNode   .isValid() ) readLayer( grasslandNode   , m_grassland    .get() );
                if ( woodlandNode    .isValid() ) readLayer( woodlandNode    , m_woodland     .get() );
                if ( builtupNode     .isValid() ) readLayer( builtupNode     , m_builtup      .get() );
                if ( railroadsNode   .isValid() ) readLayer( railroadsNode   , m_railroads    .get() );
                if ( roadsNode       .isValid() ) readLayer( roadsNode       , m_roads        .get() );
                if ( airportsNode    .isValid() ) readLayer( airportsNode    , m_airports     .get() );
                if ( waterCourseNode .isValid() ) readLayer( waterCourseNode , m_water_course .get() );
                if ( waterInlandNode .isValid() ) readLayer( waterInlandNode , m_water_inland .get() );
                if ( satelliteNode   .isValid() ) readLayer( satelliteNode   , m_satellite    .get() );
                if ( bordersNode     .isValid() ) readLayer( bordersNode     , m_borders      .get() );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Layers::readLayer( const fdm::XmlNode &node, osg::Group *parent )
{
    fdm::XmlNode fileNode = node.getFirstChildElement( "file" );

    while ( fileNode.isValid() )
    {
        fdm::XmlNode fileTextNode = fileNode.getFirstChild();

        if ( fileTextNode.isValid() && fileTextNode.isText() )
        {
            std::string file = fileTextNode.getText();

            osg::ref_ptr<osg::Node> layerNode = Models::get( file );

            if ( layerNode.valid() )
            {
                parent->addChild( layerNode.get() );
            }
        }

        fileNode = fileNode.getNextSiblingElement( "file" );
    }
}
